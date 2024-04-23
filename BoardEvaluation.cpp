#include "BoardEvaluation.h"
#include <iostream>

std::vector<std::vector<int>> kingPosition2Value = {
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,-9,-9,-9,0,0,0},
    {0,0,0,-8,-8,-8,0,0,0},
    {0,0,0,1,5,1,0,0,0}
};

std::vector<std::vector<int>> advisorPosition2Value = {
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,3,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

std::vector<std::vector<int>> bishopPosition2Value = {
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {-2,0,0,0,3,0,0,0,-2},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

std::vector<std::vector<int>> knightPosition2Value = {
    {2,2,2,8,2,8,2,2,2},
    {2,8,15,9,6,9,15,8,2},
    {4,10,11,15,11,15,11,10,4},
    {5,20,12,19,12,19,12,20,5},
    {2,12,11,15,16,15,11,12,2},
    {2,10,13,14,15,14,13,10,2},
    {4,6,10,7,10,7,10,6,4},
    {5,4,6,7,4,7,6,4,5},
    {-3,2,4,5,-10,5,4,2,-3},
    {0,-3,2,0,2,0,2,-3,0}
};

std::vector<std::vector<int>> cannonPosition2Value = {
    {4,4,0,-5,-6,-5,0,4,4},
    {2,2,0,-4,-7,-4,0,2,2},
    {1,1,0,-5,-4,-5,0,1,1},
    {0,3,3,2,4,2,3,3,0},
    {0,0,0,0,4,0,0,0,0},
    {-1,0,3,0,4,0,3,0,-1},
    {0,0,0,0,0,0,0,0,0},
    {1,0,4,3,5,3,4,0,1},
    {0,1,2,2,2,2,2,1,0},
    {0,0,1,3,3,3,1,0,0}
};

std::vector<std::vector<int>> rookPosition2Value = {
    {6,8,7,13,14,13,7,8,6},
    {6,12,9,16,33,16,9,12,6},
    {6,8,7,14,16,14,7,8,6},
    {6,13,13,16,16,16,13,13,6},
    {8,11,11,14,15,14,11,11,8},
    {8,12,12,14,15,14,12,12,8},
    {4,9,4,12,14,12,4,9,4},
    {-2,8,4,12,12,12,4,8,-2},
    {5,8,6,12,0,12,6,8,5},
    {-6,6,4,12,0,12,4,6,-6}
};

std::vector<std::vector<int>> pawnPosition2Value = {
    {0,0,0,2,4,2,0,0,0},
    {20,30,50,65,70,65,50,30,20},
    {20,30,45,55,55,55,45,30,20},
    {20,27,30,40,42,40,30,27,20},
    {10,18,22,35,40,35,22,18,10},
    {3,0,4,0,7,0,4,0,3},
    {-2,0,-2,0,6,0,-2,0,-2},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0}
};

int ChessPiecePositionEvaluation(ChessPieceType pieceType, int x, int y)
{
    switch (pieceType)
    {
    case ChessPieceType::Rook:
        return rookPosition2Value[y][x];
        break;
    case ChessPieceType::kNight:
        return knightPosition2Value[y][x];
        break;
    case ChessPieceType::Bishop:
        return bishopPosition2Value[y][x];
        break;
    case ChessPieceType::Advisor:
        return advisorPosition2Value[y][x];
        break;
    case ChessPieceType::King:
        return kingPosition2Value[y][x];
        break;
    case ChessPieceType::Cannon:
        return cannonPosition2Value[y][x];
        break;
    case ChessPieceType::Pawn:
        return pawnPosition2Value[y][x];
        break;
    default:
        std::cerr << "Invalid chess piece type" << std::endl;
        exit(2);
    }
}

int ChessPieceMoveEvaluation(const std::vector<ChessPieceType>& chessPiecesCanBeCaptured)
{
    int sum{};
    for (auto pieceType : chessPiecesCanBeCaptured)
    {
        switch (pieceType)
        {
        case ChessPieceType::Rook:
            sum += 500;
            break;
        case ChessPieceType::kNight:
            sum += 100;
            break;
        case ChessPieceType::Bishop:
            sum += 10;
            break;
        case ChessPieceType::Advisor:
            sum += 10;
            break;
        case ChessPieceType::King:
            sum += 9999;
            break;
        case ChessPieceType::Cannon:
            sum += 100;
            break;
        case ChessPieceType::Pawn:
            sum += -20;
            break;
        default:
            std::cerr << "Invalid chess piece type" << std::endl;
            exit(2);
        }
    }
    return sum;
}

int ChessPieceValueEvaluation(ChessPieceType pieceType)
{
    switch (pieceType)
    {
    case ChessPieceType::Rook:
        return 500;
        break;
    case ChessPieceType::kNight:
        return 300;
        break;
    case ChessPieceType::Bishop:
        return 30;
        break;
    case ChessPieceType::Advisor:
        return 10;
        break;
    case ChessPieceType::King:
        return 9999;
        break;
    case ChessPieceType::Cannon:
        return 300;
        break;
    case ChessPieceType::Pawn:
        return 90;
        break;
    default:
        std::cerr << "Invalid chess piece type" << std::endl;
        exit(2);
    }
}
