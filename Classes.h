#include <vector>
#include <memory>

enum Colour {
    WHITE = 0,
    BLACK = 1
};

struct Coordinates {
    int x = -1;
    int y = -1;
};

struct ChessNotationType {
    char file = 'a';
    int rank = 0;
};

class Piece;
class Board;

struct Move {
    Piece* piece = nullptr;
    Coordinates from;
    Coordinates to;

    ChessNotationType fromNotation;
    ChessNotationType toNotation;

    Piece* targetPiece = nullptr;
};



void checkForward(int dx, int dy, std::vector<Coordinates>& moves, const Board& _board, Colour colour, Coordinates pos, bool isKing);

class Board {
    Piece* squares[8][8];
public:
    const Piece* getSquare(int x, int y) const {
        return squares[x][y];
    }
    void setSquare(int x, int y, Piece* piece) {
        squares[x][y] = piece;
    }
    Coordinates findPiece(const Piece* piece) const {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (squares[i][j] == piece) {
                    return { i, j };
                }
            }
        }
        return { -1, -1 };
    }
    bool checkInside(Coordinates pos) const {
        if ((pos.x >= 0 && pos.x < 8) && (pos.y >= 0 && pos.y < 8)) {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class Piece {
protected:
    Colour colour;
    Piece(Colour _colour) {
        this->colour = _colour;
    }
public:
    virtual std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) = 0;
    virtual char getSymbol() const = 0;
    virtual void onMove() {};
    Colour getColour() const {
        return colour;
    };
};

class Pawn : public Piece {
    bool hasMoved = false;
public:
    Pawn(Colour colour) : Piece(colour) {};
    std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) override {
        std::vector<Coordinates> moves;
        if (this->colour == WHITE) {
            if (this->hasMoved == false && _board.getSquare(pos.x, pos.y + 2) == nullptr && _board.getSquare(pos.x, pos.y + 1) == nullptr) {
                moves.push_back({ pos.x,pos.y + 2 });
            }
            const Piece* foundPieceRight = _board.getSquare(pos.x + 1, pos.y + 1);
            if (_board.checkInside({ pos.x + 1, pos.y + 1 }) && foundPieceRight != nullptr) {
                if (foundPieceRight->getColour() != this->getColour()) {
                    moves.push_back({ pos.x + 1, pos.y + 1 });
                }
            }

            const Piece* foundPieceLeft = _board.getSquare(pos.x - 1, pos.y + 1);
            if (_board.checkInside({ pos.x - 1, pos.y + 1 }) && foundPieceLeft != nullptr) {
                if (foundPieceLeft->getColour() != this->getColour()) {
                    moves.push_back({ pos.x - 1, pos.y + 1 });
                }
            }
            if (_board.getSquare(pos.x, pos.y + 1) == nullptr && _board.checkInside({ pos.x, pos.y + 1 }))
                moves.push_back({ pos.x, pos.y + 1 });
        }
        else {
            if (this->hasMoved == false && _board.getSquare(pos.x, pos.y - 2) == nullptr && _board.getSquare(pos.x, pos.y - 1) == nullptr) {
                moves.push_back({ pos.x,pos.y - 2 });
            }
            const Piece* foundPieceRight = _board.getSquare(pos.x + 1, pos.y - 1);
            if (_board.checkInside({ pos.x + 1, pos.y - 1 }) && foundPieceRight != nullptr) {
                if (foundPieceRight->getColour() != this->getColour()) {
                    moves.push_back({ pos.x + 1, pos.y - 1 });
                }
            }

            const Piece* foundPieceLeft = _board.getSquare(pos.x - 1, pos.y - 1);
            if (_board.checkInside({ pos.x - 1, pos.y - 1 }) && foundPieceLeft != nullptr) {
                if (foundPieceLeft->getColour() != this->getColour()) {
                    moves.push_back({ pos.x - 1, pos.y - 1 });
                }
            }
            if (_board.getSquare(pos.x, pos.y - 1) == nullptr && _board.checkInside({ pos.x, pos.y - 1 }))
                moves.push_back({ pos.x, pos.y - 1 });
        }
        
        return moves;
    }
    char getSymbol() const override {
        return 'P';
    }

    void onMove() override {
        this->hasMoved = true;
    }

};

class Rook : public Piece {
    bool hasMoved = false;
    bool kingSide;
public:
    Rook(Colour colour, bool _kingSide) : Piece(colour) {
        this->kingSide = _kingSide;
    };
    std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) override {
        std::vector<Coordinates> moves;
        checkForward(1, 0, moves, _board, this->getColour(), pos, false);
        checkForward(-1, 0, moves, _board, this->getColour(), pos, false);
        checkForward(0, 1, moves, _board, this->getColour(), pos, false);
        checkForward(0, -1, moves, _board, this->getColour(), pos, false);
        return moves;
    }
    void rookMoved() {
        this->hasMoved = true;
    }
    char getSymbol() const override {
        return 'R';
    }
    };

class Knight : public Piece {
public:
    Knight(Colour colour) :Piece(colour) {};
    std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) override {
        std::vector<Coordinates> moves;
        std::vector<Coordinates> knighMoves({ {pos.x + 1, pos.y + 2}, {pos.x + 1,pos.y - 2}, {pos.x - 1,pos.y + 2}, {pos.x - 1,pos.y - 2}, {pos.x + 2,pos.y + 1 }, {pos.x + 2,pos.y - 1}, {pos.x - 2,pos.y + 1}, {pos.x - 2,pos.y - 1}});
        for (int i = 0; i < knighMoves.size(); i++) {
            const Piece* piece = _board.getSquare(knighMoves[i].x, knighMoves[i].y);
            if (_board.checkInside(knighMoves[i]) && (piece == nullptr || piece->getColour() != this->getColour()))
                moves.push_back(knighMoves[i]);
        }
        return moves;
    }
    char getSymbol() const override {
        return 'N';
    }
};

class Bishop : public Piece {
public:
    Bishop(Colour colour) : Piece(colour) {};
    std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) override {
        std::vector<Coordinates> moves;
        checkForward(1, 1, moves, _board, this->getColour(), pos, false);
        checkForward(1, -1, moves, _board, this->getColour(), pos, false);
        checkForward(-1, 1, moves, _board, this->getColour(), pos, false);
        checkForward(-1, -1, moves, _board, this->getColour(), pos, false);
        return moves;
    }
    char getSymbol() const override {
        return 'B';
    }
};

class Queen : public Piece {
public:
    Queen(Colour colour) : Piece(colour) {};
    std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) override {
        std::vector<Coordinates> moves;
        checkForward(1, 1, moves, _board, this->getColour(), pos, false);
        checkForward(1, -1, moves, _board, this->getColour(), pos, false);
        checkForward(-1, 1, moves, _board, this->getColour(), pos, false);
        checkForward(-1, -1, moves, _board, this->getColour(), pos, false);
        checkForward(1, 0, moves, _board, this->getColour(), pos, false);
        checkForward(-1, 0, moves, _board, this->getColour(), pos, false);
        checkForward(0, 1, moves, _board, this->getColour(), pos, false);
        checkForward(0, -1, moves, _board, this->getColour(), pos, false);
        return moves;
    }
    char getSymbol() const override {
        return 'Q';
    }
};

class King : public Piece {
public:
    bool inCheck;
    King(Colour colour) : Piece(colour) {
        this->inCheck = false;
    }
    std::vector<Coordinates> LegalMoves(const Board& _board, Coordinates pos) override {
        std::vector<Coordinates> moves;
        checkForward(1, 1, moves, _board, this->getColour(), pos, true);
        checkForward(1, -1, moves, _board, this->getColour(), pos, true);
        checkForward(-1, 1, moves, _board, this->getColour(), pos, true);
        checkForward(-1, -1, moves, _board, this->getColour(), pos, true);
        checkForward(1, 0, moves, _board, this->getColour(), pos, true);
        checkForward(-1, 0, moves, _board, this->getColour(), pos, true);
        checkForward(0, 1, moves, _board, this->getColour(), pos, true);
        checkForward(0, -1, moves, _board, this->getColour(), pos, true);
        return moves;
    }
    void checkKing() {
        this->inCheck = true;
    }
    char getSymbol() const override {
        return 'K';
    }
    Colour getColour() const {
        return this->colour;
    }
};

void checkForward(int dx, int dy, std::vector<Coordinates>& moves, const Board& _board, Colour colour, Coordinates pos, bool isKing) {
    pos.x += dx;
    pos.y += dy;
    while (_board.checkInside(pos)) {
        const Piece* checkSquare = _board.getSquare(pos.x, pos.y);
        if (checkSquare == nullptr) {
            moves.push_back(pos);
        }
        else
        {
            if (checkSquare->getColour() != colour) {
                moves.push_back(pos);
            }
            break;
        }
        if (isKing)
        {
            break;
        }
        pos.x += dx;
        pos.y += dy;
    }
}