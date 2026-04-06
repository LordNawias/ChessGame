#include <iostream>
#include <vector>
#include <memory>
#include "Classes.h"
#include <string>

enum PieceType {
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};


void initializeBoard(Board& _board);
void setUpBoard(Board& _board, std::vector<std::unique_ptr<Piece>>& whitePieces, std::vector<std::unique_ptr<Piece>>& blackPieces, Piece*& whiteKing, Piece*& blackKing);
void drawBoard(Board& _board);
std::unique_ptr<Piece> createPiece(PieceType type, Colour colour, int index);
void mainLoop(Board& _board, std::vector<std::unique_ptr<Piece>>& whitePieces, std::vector<std::unique_ptr<Piece>>& blackPieces, Coordinates& whiteKingPos, Coordinates& blackKingPos, Piece* whiteKingPtr, Piece* blackKingPtr);
ChessNotationType chessNotation(int x, int y);
Coordinates fromNotationToPos(char file, char rank);
int handleInput(std::string input, std::vector<Move>& allMoves);
void makeMove(Move& move, Board& _board, Coordinates& whiteKingPos, Coordinates& blackKingPos, Piece* whiteKingPtr, Piece* blackKingPtr);
bool isKingInCheck(Coordinates playerKingPos, std::vector<Coordinates> enemyMoves);


int main()
{
    Board board;
    std::vector<std::unique_ptr<Piece>> whitePieces;
    std::vector<std::unique_ptr<Piece>> blackPieces;
    Piece* whiteKingPointer = nullptr;
    Piece* blackKingPoitner = nullptr;
    initializeBoard(board);
    setUpBoard(board, whitePieces, blackPieces, whiteKingPointer, blackKingPoitner);
    Coordinates whiteKingPos = board.findPiece(whiteKingPointer);
    Coordinates blackKingPos = board.findPiece(blackKingPoitner);
    drawBoard(board);
    mainLoop(board, whitePieces, blackPieces, whiteKingPos, blackKingPos, whiteKingPointer, blackKingPoitner);
    return 0;
}

void initializeBoard(Board& _board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            _board.setSquare(j, i, nullptr);
        }
    }
}

void setUpBoard(Board& _board, std::vector<std::unique_ptr<Piece>>& whitePieces, std::vector<std::unique_ptr<Piece>>& blackPieces, Piece*& whiteKing, Piece*& blackKing) {
    for (int i = 0; i < 8; i++) {
        auto pawn = std::make_unique<Pawn>(BLACK);
        _board.setSquare(i, 6, pawn.get());
        blackPieces.push_back(std::move(pawn));
        pawn = std::make_unique<Pawn>(WHITE);
        _board.setSquare(i, 1, pawn.get());
        whitePieces.push_back(std::move(pawn));
    };
    
    std::vector<PieceType> lineup = {
        ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
    };
    for (int i = 0; i < 8; i++) {
        std::unique_ptr<Piece> piece = createPiece(lineup[i], WHITE, i);
        _board.setSquare(i, 0, piece.get());
        if (lineup[i] == KING) {
            whiteKing = piece.get();
        }
        whitePieces.push_back(std::move(piece));
    }

    for (int i = 0; i < 8; i++) {
        std::unique_ptr<Piece> piece = createPiece(lineup[i], BLACK, i);
        _board.setSquare(i, 7, piece.get());
        if (lineup[i] == KING) {
            blackKing = piece.get();
        }
        blackPieces.push_back(std::move(piece));
    }
}

void drawBoard(Board& _board) {
    for (int i = 7; i >= 0; i--) {
        for (int j = 0; j < 8; j++) {
            const Piece* piece = _board.getSquare(j, i);
            if (piece == nullptr) {
                std::cout << "0 ";
            }
            else {
                std::cout << piece->getSymbol() << " ";
            }
        }
        std::cout << std::endl;
    }
}


std::unique_ptr<Piece> createPiece(PieceType type, Colour colour, int index) {
    switch (type) {
    case ROOK:
        return std::make_unique<Rook>(colour, index == 7);
    case KNIGHT:
        return std::make_unique<Knight>(colour);
    case BISHOP:
        return std::make_unique<Bishop>(colour);
    case QUEEN:
        return std::make_unique<Queen>(colour);
    case KING:
        return std::make_unique<King>(colour);
    }
    return nullptr;
}


void mainLoop(Board& _board, std::vector<std::unique_ptr<Piece>>& whitePieces, std::vector<std::unique_ptr<Piece>>& blackPieces, Coordinates& whiteKingPos, Coordinates& blackKingPos, Piece* whiteKingPtr, Piece* blackKingPtr) {
    bool activeGame = true;
    Colour activePlayer = WHITE;
    std::string input;
    std::vector<Move>allWhiteMoves;
    std::vector<Move>allBlackMoves;
    while (activeGame) {
        for (const auto& whitePiece : whitePieces) {
            Coordinates pos = _board.findPiece(whitePiece.get());
            std::vector<Coordinates> startingMoves = whitePiece->LegalMoves(_board, pos);

        }
        /*
        allMoves.clear();
        system("cls");
        drawBoard(_board);
        if (activePlayer == WHITE) {
            for (const auto& i : whitePieces) {
                Coordinates pos = _board.findPiece(i.get());
                auto pseudoLegalMoves = i->LegalMoves(_board, pos);
                for (const auto& m : pseudoLegalMoves) {
                    Move move;
                    move.piece = i.get();
                    move.from = pos;
                    move.to = m;
                    move.fromNotation = chessNotation(pos.x, pos.y);
                    move.toNotation = chessNotation(m.x, m.y);
                    makeMove(move, _board, whiteKingPos, blackKingPos, whiteKingPtr, blackKingPtr);
                    if (!isKingInCheck(whiteKingPos, pseudoLegalMoves)) {
                        allMoves.push_back(move);
                        std::cout << move.fromNotation.file << move.fromNotation.rank << move.toNotation.file << move.toNotation.rank << std::endl;
                    }
                    undoMove(move, _board, whiteKingPos, blackKingPos, whiteKingPtr, blackKingPtr);
                }
            }
        }
        int moveIndex = 0;
        do {
            std::cin >> input;
            moveIndex = handleInput(input, allMoves);
        } while (moveIndex == -1);
        Move move = allMoves[moveIndex];
        _board.setSquare(move.to.x, move.to.y, move.piece);
        _board.setSquare(move.from.x, move.from.y, nullptr);
        move.piece->onMove();
        */
    }
}

void makeMove(Move& move, Board& _board, Coordinates& whiteKingPos, Coordinates& blackKingPos, Piece* whiteKingPtr, Piece* blackKingPtr) {
    if (move.piece = whiteKingPtr) {
        whiteKingPos = move.to;
    }
    if (move.piece = blackKingPtr) {
        blackKingPos = move.to;
    }
    move.targetPiece = _board.getSquare(move.to.x, move.to.y);
    _board.setSquare(move.to.x, move.to.y, move.piece);
    _board.setSquare(move.from.x, move.from.y, nullptr);
}

void undoMove(Move& move, Board& _board, Coordinates& whiteKingPos, Coordinates& blackKingPos, Piece* whiteKingPtr, Piece* blackKingPtr) {
    if (move.piece = whiteKingPtr) {
        whiteKingPos = move.from;
    }
    if (move.piece = blackKingPtr) {
        blackKingPos = move.from;
    }

    _board.setSquare(move.from.x, move.from.y, move.piece);
    _board.setSquare(move.to.x, move.to.y, move.targetPiece);
}

ChessNotationType chessNotation(int x, int y) {
    char file='a';
    switch (x)
    {
    case 0:
        file = 'a';
        break;
    case 1:
        file = 'b';
        break;
    case 2:
        file = 'c';
        break;
    case 3:
        file = 'd';
        break;
    case 4:
        file = 'e';
        break;
    case 5:
        file = 'f';
        break;
    case 6:
        file = 'g';
        break;
    case 7:
        file = 'h';
        break;
    }
    int rank = y + 1;
    return { file, rank };
}

int handleInput(std::string input, std::vector<Move>& allMoves) {
    if (input.size() != 4) {
        return -1;
    }
    int index = -1;
    for (int i = 0; i < allMoves.size(); i++) {
        if (input[0] == allMoves[i].fromNotation.file && input[1] - '0' == allMoves[i].fromNotation.rank && input[2] == allMoves[i].toNotation.file && input[3] - '0' == allMoves[i].toNotation.rank) {
            index = i;
            break;
        }
    }
    return index;
}

Coordinates fromNotationToPos(char file, char rank) {
    int x=0;
    int y = (rank - '0') - 1;
    switch (file) {
    case 'a':
        x = 0;
        break;
    case 'b':
        x = 1;
        break;
    case 'c':
        x = 2;
        break;
    case 'd':
        x = 3;
        break;
    case 'e':
        x = 4;
        break;
    case 'f':
        x = 5;
        break;
    case 'g':
        x = 6;
        break;
    case 'h':
        x = 7;
        break;
    }
    return { x,y };
}

bool isKingInCheck(Coordinates playerKingPos, std::vector<Coordinates> enemyMoves) {
    bool isCheck = false;
    for (auto& move : enemyMoves) {
        if (move.x==playerKingPos.x && move.y==playerKingPos.y) {
            isCheck = true;
            break;
        }
    }
    return isCheck;
}