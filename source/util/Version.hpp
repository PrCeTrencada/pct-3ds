#pragma once
#ifndef VERSION_HPP
#define VERSION_HPP

#include <string>

#include "../common/commonValues.hpp"

using namespace std;

namespace Version
{
	class Data
	{
	public:

		Data()
		{
			m_major = CURRENT_VERSION_MAJOR;
			m_minor = CURRENT_VERSION_MINOR;
			m_micro = CURRENT_VERSION_MICRO;
			m_note = "beta";
		}
		
		Data(size_t major, size_t minor, size_t micro, string note)
		{
			m_major = major;
			m_minor = minor;
			m_micro = micro;
			m_note = note;
		}

		// Get the version in the format "vX.Y.Z-note" or "vX.Y.X"
		Data(string version)
		{
			size_t pos = version.find(".");
			m_major = stoi(version.substr(1, pos - 1));
			size_t pos2 = version.find(".", pos + 1);
			m_minor = stoi(version.substr(pos + 1, pos2 - pos - 1));
			size_t pos3 = version.find("-", pos2 + 1);
			if (pos3 == string::npos) {
				m_micro = stoi(version.substr(pos2 + 1));
				m_note = "";
			}
			else {
				m_micro = stoi(version.substr(pos2 + 1, pos3 - pos2 - 1));
				m_note = version.substr(pos3 + 1);
			}
		}

		const string getVersion()
		{
			return "v" + to_string(m_major) + "." + to_string(m_minor) + "." + to_string(m_micro);
		}

		const string getVersionWithNote()
		{
			return "v" + to_string(m_major) + "." + to_string(m_minor) + "." + to_string(m_micro) + "-" + m_note;
		}

		// Check if the version passed is greater than the current version
		bool operator<(Data& other)
		{
			if (m_major < other.m_major) return true;
			if (m_major > other.m_major) return false;
			if (m_minor < other.m_minor) return true;
			if (m_minor > other.m_minor) return false;
			if (m_micro < other.m_micro) return true;
			if (m_micro > other.m_micro) return false;
			return false;
		}

		// Check if the version passed is less than the current version
		bool operator>(Data& other)
		{
			if (m_major > other.m_major) return true;
			if (m_major < other.m_major) return false;
			if (m_minor > other.m_minor) return true;
			if (m_minor < other.m_minor) return false;
			if (m_micro > other.m_micro) return true;
			if (m_micro < other.m_micro) return false;
			return false;
		}

	private:
		size_t m_major;
		size_t m_minor;
		size_t m_micro;
		string m_note;
	};

	Version::Data getCurrentVersionData();

	Version::Data getNewVersionData();
}

#endif // !VERSION_HPP
