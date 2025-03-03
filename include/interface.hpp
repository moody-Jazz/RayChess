#include "global.hpp"
#include "button.hpp"
#include "engine.hpp"
#include "chessboard.hpp"
#include <memory>

class Interface
{
public:
    Interface(Board& board, Engine& engine);

    Board& board;
    Engine& engine;
    uint64_t nodesSearched;
    std::string bestMove;
    // Utility Functions
    void drawSidePanel() const;

    void boardInteractionHandler();
    void sidePanelInteractionHandler();
    void interactionHandler();

private:
    size_t sidePanelX_;
    size_t leftPadding_;
    size_t topPadding_;
    std::string logo_;
    Button btnCopyFEN_;
    Button btnStart_;
    Button btnPlayWhite_;
    Button btnPlayBlack_;
    char currPieceType_;
    int clickedOnRow_;
    int clickedOnCol_;
    bool pieceSelected_;
};