/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef VARIABLES_H_
#define VARIABLES_H_

#include "common/hashmap.h"
#include "common/savefile.h"
#include "common/serializer.h"

#include "engines/myst3/myst3.h"

namespace Myst3 {

// View type
enum ViewType {
	kCube = 1,
	kFrame = 2,
	kMenu = 3
};

#define DECLARE_VAR(num, name) \
	void set##name(int32 value) { engineSet(num, value); } \
	int32 get##name() { return engineGet(num); }

class GameState {
public:
	GameState(Myst3Engine *vm);
	virtual ~GameState();

	void newGame();
	bool load(const Common::String &file);
	bool save(Common::OutSaveFile *save);

	int32 getVar(uint16 var);
	void setVar(uint16 var, int32 value);
	bool evaluate(int16 condition);
	int32 valueOrVarValue(int16 value);

	const Common::String describeVar(uint16 var);
	const Common::String describeCondition(int16 condition);

	DECLARE_VAR(61, LocationAge)
	DECLARE_VAR(62, LocationRoom)
	DECLARE_VAR(63, LocationNode)
	DECLARE_VAR(64, BookSavedAge)
	DECLARE_VAR(65, BookSavedRoom)
	DECLARE_VAR(66, BookSavedNode)
	DECLARE_VAR(67, MenuSavedAge)
	DECLARE_VAR(68, MenuSavedRoom)
	DECLARE_VAR(69, MenuSavedNode)

	DECLARE_VAR(115, SunspotIntensity)
	DECLARE_VAR(116, SunspotColor)
	DECLARE_VAR(117, SunspotRadius)

	DECLARE_VAR(142, MovieStartFrame)
	DECLARE_VAR(143, MovieEndFrame)
	DECLARE_VAR(149, MovieConditionBit)
	DECLARE_VAR(150, MoviePreloadToMemory)
	DECLARE_VAR(151, MovieScriptDriven)
	DECLARE_VAR(152, MovieNextFrameSetVar)
	DECLARE_VAR(153, MovieNextFrameGetVar)
	DECLARE_VAR(154, MovieStartFrameVar)
	DECLARE_VAR(155, MovieEndFrameVar)
	DECLARE_VAR(160, MovieSynchronized)
	DECLARE_VAR(163, MovieOverrideCondition)
	DECLARE_VAR(164, MovieUVar)
	DECLARE_VAR(165, MovieVVar)
	DECLARE_VAR(166, MovieOverridePosition)
	DECLARE_VAR(167, MovieOverridePosU)
	DECLARE_VAR(168, MovieOverridePosV)
	DECLARE_VAR(173, MoviePlayingVar)

	DECLARE_VAR(189, LocationNextNode)
	DECLARE_VAR(190, LocationNextRoom)
	DECLARE_VAR(191, LocationNextAge)

	DECLARE_VAR(277, JournalAtrusState)
	DECLARE_VAR(279, JournalSaavedroState)
	DECLARE_VAR(280, JournalSaavedroClosed)
	DECLARE_VAR(281, JournalSaavedroOpen)
	DECLARE_VAR(282, JournalSaavedroLastPage)
	DECLARE_VAR(283, JournalSaavedroChapter)
	DECLARE_VAR(284, JournalSaavedroPageInChapter)
	DECLARE_VAR(480, BookStateTomahna)
	DECLARE_VAR(481, BookStateReleeshahn)

	DECLARE_VAR(1337, MenuEscapePressed)
	DECLARE_VAR(1338, MenuNextAction)
	DECLARE_VAR(1339, MenuLoadBack)
	DECLARE_VAR(1340, MenuSaveBack)
	DECLARE_VAR(1341, MenuSaveAction)
	DECLARE_VAR(1342, MenuOptionsBack)

	DECLARE_VAR(1350, MenuSaveLoadPageLeft)
	DECLARE_VAR(1351, MenuSaveLoadPageRight)
	DECLARE_VAR(1352, MenuSaveLoadSelectedItem)
	DECLARE_VAR(1353, MenuSaveLoadCurrentPage)

	uint getFrameCount() { return _data.currentFrame; }
	void incFrameCount() { _data.currentFrame++; }

	ViewType getViewType() { return static_cast<ViewType>(_data.currentNodeType); }
	void setViewType(ViewType t) { _data.currentNodeType = t; }

	float getLookAtPitch() { return _data.lookatPitch; }
	float getLookAtHeading() { return _data.lookatHeading; }
	void lookAt(float pitch, float heading) { _data.lookatPitch = pitch; _data.lookatHeading = heading; }

	Common::Array<uint16> getInventory();
	void updateInventory(const Common::Array<uint16> &items);

private:
	Myst3Engine *_vm;

	static const uint32 kSaveVersion = 148;

	struct StateData {
		uint32 version;
		uint32 gameRunning;
		uint32 currentFrame;
		uint32 dword_4C2C3C;
		uint32 dword_4C2C40;
		uint32 dword_4C2C44;
		uint32 dword_4C2C48;
		uint32 dword_4C2C4C;
		uint32 dword_4C2C50;
		uint32 dword_4C2C54;
		uint32 dword_4C2C58;
		uint32 dword_4C2C5C;
		uint32 dword_4C2C60;
		uint32 currentNodeType;
		float lookatPitch;
		float lookatHeading;
		float lookatFOV;
		float pitchOffset;
		float headingOffset;
		uint32 limitCubeCamera;
		float minPitch;
		float maxPitch;
		float minHeading;
		float maxHeading;
		uint32  dword_4C2C90;
		int32 vars[2048];
		uint32 inventoryCount;
		uint32 inventoryList[7];
		int8 zipDestinations[256];
	};

	StateData _data;

	struct Description {
		Description() {}
		Description(uint16 v, const char *n, bool u) : var(v), name(n), unknown(u) {}

		uint16 var;
		const char *name;
		bool unknown;
	};

	Common::HashMap<uint16, Description> _descriptions;

	void syncWithSaveGame(Common::Serializer &s);

	void checkRange(uint16 var);

	int32 engineGet(uint16 var);
	void engineSet(uint16 var, int32 value);
};

} /* namespace Myst3 */
#endif /* VARIABLES_H_ */