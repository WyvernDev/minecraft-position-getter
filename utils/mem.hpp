#pragma once
#include <Windows.h>
#include <string_view>
#include <optional>
#include <TlHelp32.h>
#include <stdexcept>
#include <format>
#include <vector>

/*basic external memory class, expand upon this if you wish, all this actually does is read memory.*/

class process
{
private:
	std::int32_t m_pid = 0;
	HANDLE m_handle = nullptr;

	inline std::optional<std::int32_t> get_pid(std::string_view proc_name)
	{
		auto result = std::make_optional<std::int32_t>();
		result = std::nullopt;

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (!snapshot)
			return std::nullopt;

		auto proc_entry = PROCESSENTRY32{};
		proc_entry.dwSize = sizeof(proc_entry);

		if (Process32First(snapshot, &proc_entry))
		{
			do
			{
				if (std::string_view(proc_entry.szExeFile).contains(proc_name))
				{
					result = proc_entry.th32ProcessID;
					break;
				}
			} while (Process32Next(snapshot, &proc_entry));
		}

		CloseHandle(snapshot);
		return result;
	}

public:
	process(std::string_view process_name)
	{
		auto pid = get_pid(process_name);

		if (!pid.has_value())
			throw std::runtime_error(std::format("Unable to find process ID for {}.", process_name));

		m_pid = pid.value();
		m_handle = OpenProcess(PROCESS_ALL_ACCESS, false, m_pid);
	}
	process(std::int32_t pid)
		: m_pid(pid), m_handle(OpenProcess(PROCESS_ALL_ACCESS, false, pid)) {}
	process(HANDLE h)
		: m_handle(h), m_pid(GetProcessId(h)) {}
	
	~process()
	{
		if (m_handle)
			CloseHandle(m_handle);
	}

	template <class t>
	inline t read(std::uintptr_t addr) const
	{
		t buf{};
		ReadProcessMemory(m_handle, reinterpret_cast<void*>(addr), &buf, sizeof(buf), nullptr);
		return buf;
	}

	inline std::uintptr_t follow_ptr_chain(std::uintptr_t base, std::vector<std::uintptr_t> offsets) const
	{
		for (const auto& off : offsets)
		{
			base = read<std::uintptr_t>(base);
			base += off;
		}

		return base;
	}

	inline std::optional<std::uintptr_t> get_module_base(std::string_view mod_name) const
	{
		auto result = std::make_optional<std::uintptr_t>();
		result = std::nullopt;

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_pid);
		auto entry = MODULEENTRY32{};
		entry.dwSize = sizeof(entry);

		if (Module32First(snapshot, &entry))
		{
			do
			{
				if (entry.th32ProcessID == m_pid && std::string_view(entry.szExePath).contains(mod_name))
				{
					result = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
					break;
				}
			} while (Module32Next(snapshot, &entry));
		}

		CloseHandle(snapshot);
		return result;
	}
	

};
