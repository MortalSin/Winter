/*
 *  Winter is a UCI chess engine.
 *
 *  Copyright (C) 2016 Jonas Kuratli, Jonathan Maurer, Jonathan Rosenthal
 *  Copyright (C) 2017-2018 Jonathan Rosenthal
 *
 *  Winter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Winter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * feature_indexes.h
 *
 *  Created on: May 16, 2017
 *      Author: Jonathan Rosenthal
 */

#ifndef SRC_GENERAL_FEATURE_INDEXES_H_
#define SRC_GENERAL_FEATURE_INDEXES_H_

#include "types.h"
#include <array>

const int kPSTindex[64] = {
    0, 1, 2, 3, 3, 2, 1, 0,
    1, 4, 5, 6, 6, 5, 4, 1,
    2, 5, 7, 8, 8, 7, 5, 2,
    3, 6, 8, 9, 9, 8, 6, 3,
    3, 6, 8, 9, 9, 8, 6, 3,
    2, 5, 7, 8, 8, 7, 5, 2,
    1, 4, 5, 6, 6, 5, 4, 1,
    0, 1, 2, 3, 3, 2, 1, 0
};

struct FeatureInfo {
  FeatureInfo(std::string info_, int idx_, Score min = kMinScore,
              Score max = kMaxScore, int encoded_info_ = 0) :
    info(info_), idx(idx_), min_value(min),
    max_value(max), encoded_info(encoded_info_){ };
  const std::string info;
  const int idx;
  const Score min_value;
  const Score max_value;
  const int encoded_info;
};

namespace positional_features {

const int kMinorKingAttackWeight = 2;
const int kRookKingAttackWeight = 3;
const int kQueenKingAttackWeight = 5;

const int kKingAttackerScaling[7] = {0, 0, 4, 6, 7, 7, 8};

const int kKingPSTIndex = 0;
const int kKnightPSTIndex = kKingPSTIndex + 10;
const int kBaseValueIndex = kKnightPSTIndex + 10;
const int kBishopPairIndex = kBaseValueIndex + 5;//No King
const int kActivityBonusIndex = kBishopPairIndex + 1;
const int kAbstractActivityIndex = kActivityBonusIndex + 5;//No King
const int kPassedPawnBonusIndex = kAbstractActivityIndex + 5;//No king
//const int kPassedPawnUnblocked = kPassedPawnBonusIndex + 6;
//const int kPassedPawnPushBonusIndex = kPassedPawnBonusIndex + 6;
const int kDoublePawnPenaltyIndex = kPassedPawnBonusIndex + 12;//kRookBehindPasser + 1;
const int kIsolatedPawnIndex = kDoublePawnPenaltyIndex + 1;
const int kTempoBonusIndex = kIsolatedPawnIndex + 1;
const int kDirectPawnShieldBonus = kTempoBonusIndex + 1;
const int kKingVectorExposure = kDirectPawnShieldBonus + 1;
const int kKnightOutpost = kKingVectorExposure + 2;
const int kCastlingRight = kKnightOutpost + 1;
const int kPawnAttackIndex = kCastlingRight + 1;
const int kMinorAttackIndex = kPawnAttackIndex + 1;
const int kKingDefenceDistance = kMinorAttackIndex + 1;
const int kKingAttackDistance = kKingDefenceDistance + 4; // No pawns, no king
const int kKingAttack = kKingAttackDistance + 4; // No pawns, no king
const int kKnightSquaresIndex = kKingAttack + kQueen - kPawn;
const int kBishopMobility = kKnightSquaresIndex + 9;
const int kRookMobility = kBishopMobility + 14;
const int kRookOpenFile = kRookMobility + 15;
const int kQueenMobility = kRookOpenFile + 1;
const int kSafeChecks = kQueenMobility + 28;
const int kUnSafeChecks = kSafeChecks + 1;
const int kUnprotectedPieces = kUnSafeChecks + 1;
const int kBishopVsKingPosition = kUnprotectedPieces + 1;
const int kOpposedPawn = kBishopVsKingPosition + 36;
const int kUnopposedPawn = kOpposedPawn + (2 * 2 * 6);
const int kCoveredPassedPawn = kUnopposedPawn + (2 * 2 * 6);
const int kUncoveredPassedPawn = kCoveredPassedPawn + (2 * 2 * 6);
const int kPawnEvalIndex = kOpposedPawn;
const int kNumFeatures = kUncoveredPassedPawn + (2 * 2 * 6);



const std::array<FeatureInfo, 33> kFeatureInfos = {{
    FeatureInfo("King Piece Square Table", kKingPSTIndex),
    FeatureInfo("Knight Piece Square Table", kKnightPSTIndex, 0, kMaxScore),
    FeatureInfo("Piece Base Value", kBaseValueIndex, kMinScore, kMaxScore),
    FeatureInfo("Bishop Pair", kBishopPairIndex, 0, kMaxScore),
    FeatureInfo("Piece Activity Bonus", kActivityBonusIndex, 0, kMaxScore),
    FeatureInfo("Abstract Piece Activity Bonus", kAbstractActivityIndex, 0, kMaxScore),
    FeatureInfo("Passed Pawn Bonus", kPassedPawnBonusIndex, 0, kMaxScore),
    //FeatureInfo("Passed Pawn Unblocked", kPassedPawnUnblocked),
    //FeatureInfo("Rook Behind Passed Pawn Bonus", kRookBehindPasser, -5, kMaxScore),
    FeatureInfo("Double Pawn Penalty", kDoublePawnPenaltyIndex, kMinScore, 0),
    FeatureInfo("Isolated Pawn", kIsolatedPawnIndex, kMinScore, 0),
    FeatureInfo("Tempo Bonus", kTempoBonusIndex, 0, kMaxScore),
    FeatureInfo("Direct Pawn Shield Bonus", kDirectPawnShieldBonus),
    FeatureInfo("King Vector Exposure", kKingVectorExposure),
    FeatureInfo("Knight Outpost", kKnightOutpost),
    FeatureInfo("Castling Right", kCastlingRight, 0, kMaxScore),
    FeatureInfo("Pawn Attack", kPawnAttackIndex, 0, kMaxScore),
    FeatureInfo("Minor Attack", kMinorAttackIndex, 0, kMaxScore),
    FeatureInfo("King Defence Distance", kKingDefenceDistance),
    FeatureInfo("King Attack Distance", kKingAttackDistance, kMinScore, 0),
    FeatureInfo("King Attack Danger", kKingAttack, 0, kMaxScore),
    FeatureInfo("Safe Knight Square Count", kKnightSquaresIndex, kMinScore, kMaxScore, 1),
    FeatureInfo("Bishop Mobility", kBishopMobility, kMinScore, kMaxScore, 1),
    FeatureInfo("Rook Mobility", kRookMobility, kMinScore, kMaxScore, 1),
    FeatureInfo("Rook on Open File", kRookOpenFile, 0, kMaxScore),
    FeatureInfo("Queen Mobility", kQueenMobility, kMinScore, kMaxScore, 1),
    FeatureInfo("Safe Checks", kSafeChecks),
    FeatureInfo("Unsafe Checks", kUnSafeChecks),
    FeatureInfo("Unprotected Pieces", kUnprotectedPieces),
    FeatureInfo("Bishop's Position Relative To King", kBishopVsKingPosition),
    FeatureInfo("Opposed Pawns", kOpposedPawn),
    FeatureInfo("Unopposed Pawns", kUnopposedPawn),
    FeatureInfo("Covered Passed Pawns", kCoveredPassedPawn),
    FeatureInfo("Uncovered Passed Pawns", kUncoveredPassedPawn),
    FeatureInfo("Num Features Placeholder", kNumFeatures)
}};

}

namespace move_features {

const int kPWIHashMove = 0;
const int kPWIKiller = kPWIHashMove + 1;
const int kPWICounterMove = kPWIKiller + 2;
const int kPWIPieceTypeXTargetPieceType = kPWICounterMove + 1;
const int kPWIMoveType = kPWIPieceTypeXTargetPieceType + 36;
const int kPWIMoveSource = kPWIMoveType + 9;
const int kPWIMoveDestination = kPWIMoveSource + 10;
const int kPWIKnightMoveSource = kPWIMoveDestination + 10;
const int kPWIKnightMoveDestination = kPWIKnightMoveSource + 10;
const int kPWICaptureLastMoved = kPWIKnightMoveDestination + 10;
const int kPWISEE = kPWICaptureLastMoved + 1;
const int kPWIGivesCheck = kPWISEE + 2;
const int kPWITabooDestination = kPWIGivesCheck + 2;
const int kPWIForcingChanges = kPWITabooDestination + 1;
const int kPWIPawnRankDestination = kPWIForcingChanges + 4;
//const int kNumMoveProbabilityFeatures = kPWIForcingChanges + 4;
const int kPWIPassedRankDestination = kPWIPawnRankDestination + 6;
const int kPWIPawnAttack = kPWIPassedRankDestination + 6;
const int kPWIPieceUnderAttack = kPWIPawnAttack + 1;
const int kPWICMH = kPWIPieceUnderAttack + 2;
const int kNumMoveProbabilityFeatures = kPWICMH+ 1;

const std::array<FeatureInfo, 20> kFeatureInfos = {{
    FeatureInfo("Hash Move", kPWIHashMove),
    FeatureInfo("Killer Move", kPWIKiller),
    FeatureInfo("Counter Move", kPWICounterMove),
    FeatureInfo("Moving and Target Piece Type", kPWIPieceTypeXTargetPieceType),
    FeatureInfo("Move Type", kPWIMoveType),
    FeatureInfo("Move Source", kPWIMoveSource),
    FeatureInfo("Move Destination", kPWIMoveDestination),
    FeatureInfo("Knight Move Source", kPWIKnightMoveSource),
    FeatureInfo("Knight Move Destination", kPWIKnightMoveDestination),
    FeatureInfo("Move Captures Last Moved Piece", kPWICaptureLastMoved),
    FeatureInfo("Move SEE", kPWISEE),
    FeatureInfo("Move Gives Check", kPWIGivesCheck),
    FeatureInfo("Move Destination Taboo", kPWITabooDestination),
    FeatureInfo("Changes Between Non-/Forcing States", kPWIForcingChanges),
    FeatureInfo("Rank of pawn destination", kPWIPawnRankDestination),
    FeatureInfo("Rank of passed pawn destination", kPWIPassedRankDestination),
    FeatureInfo("Pawn move attacks a piece", kPWIPawnAttack),
    FeatureInfo("Piece under attack", kPWIPieceUnderAttack),
    FeatureInfo("CMH", kPWICMH),
    FeatureInfo("Num Features Placeholder", kNumMoveProbabilityFeatures)
}};

}


#endif /* SRC_GENERAL_FEATURE_INDEXES_H_ */
