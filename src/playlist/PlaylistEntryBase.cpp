/*
 *   Playlist Entry Base Class for Falcon Player (FPP)
 *
 *   Copyright (C) 2016 the Falcon Player Developers
 *      Initial development by:
 *      - David Pitts (dpitts)
 *      - Tony Mace (MyKroFt)
 *      - Mathew Mrosko (Materdaddy)
 *      - Chris Pinkham (CaptainMurdoch)
 *      For additional credits and developers, see credits.php.
 *
 *   The Falcon Pi Player (FPP) is free software; you can redistribute it
 *   and/or modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "log.h"
#include "PlaylistEntryBase.h"

/*
 *
 */
PlaylistEntryBase::PlaylistEntryBase()
  : m_enabled(1),
	m_isStarted(0),
	m_isPlaying(0),
	m_isFinished(0),
	m_playOnce(0),
	m_playCount(0)
{
	m_type = "Base";
}

/*
 *
 */
PlaylistEntryBase::~PlaylistEntryBase()
{
}

/*
 *
 */
int PlaylistEntryBase::Init(Json::Value &config)
{
	LogDebug(VB_PLAYLIST, "PlaylistEntryBase::Init(): '%s'\n", config["type"].asString().c_str());

	if (config.isMember("enabled"))
		m_enabled = config["enabled"].asInt();

	m_isStarted = 0;
	m_isPlaying = 0;
	m_isFinished = 0;

	return 1;
}

/*
 *
 */
int PlaylistEntryBase::CanPlay(void)
{
	if (m_playOnce && (m_playCount > 0))
	{
		LogDebug(VB_PLAYLIST, "%s item exceeds play count\n", m_type.c_str());
		return 0;
	}

	if (!m_enabled)
	{
		LogDebug(VB_PLAYLIST, "%s item disabled\n", m_type.c_str());
		return 0;
	}

	return 1;
}

/*
 *
 */
int PlaylistEntryBase::StartPlaying(void)
{
	LogDebug(VB_PLAYLIST, "PlaylistEntryBase::StartPlaying()\n");

	m_isStarted = 1;
	m_isPlaying = 1;
	m_isFinished = 0;
	m_playCount++;

	return 1;
}

/*
 *
 */
void PlaylistEntryBase::FinishPlay(void)
{
	m_isStarted = 1;
	m_isPlaying = 0;
	m_isFinished = 1;
}

/*
 *
 */
int PlaylistEntryBase::IsStarted(void)
{
	return m_isStarted;
}

/*
 *
 */
int PlaylistEntryBase::IsPlaying(void)
{
	if (m_isStarted && m_isPlaying)
		return 1;

	return 0;
}

/*
 *
 */
int PlaylistEntryBase::IsFinished(void)
{
	if (m_isStarted && !m_isPlaying && m_isFinished)
		return 1;

	return 0;
}

/*
 *
 */
int PlaylistEntryBase::Process(void)
{
	return 1;
}

/*
 *
 */
int PlaylistEntryBase::Stop(void)
{
	m_isPlaying = 0;
	m_isFinished = 1;

	return 1;
}

/*
 *
 */
int PlaylistEntryBase::HandleSigChild(pid_t pid)
{
	return 0;
}

/*
 *
 */
void PlaylistEntryBase::Dump(void)
{
	LogDebug(VB_PLAYLIST, "Entry: %s\n", m_type.c_str());
}

/*
 *
 */
Json::Value PlaylistEntryBase::GetConfig(void)
{
	Json::Value result;

	result["type"]       = m_type;
	result["enabled"]    = m_enabled;
	result["isStarted"]  = m_isStarted;
	result["isPlaying"]  = m_isPlaying;
	result["isFinished"] = m_isFinished;
	result["playOnce"]   = m_playOnce;
	result["playCount"]  = m_playCount;

	return result;
}
