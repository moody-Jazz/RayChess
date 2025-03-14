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
    uint64_t nodesSearched;                 // Stores the number of positions searched by the engine to arrive at best move
    std::string bestMove;                   // Stores the best move found by the engine
    // Utility Functions
    void drawSidePanel() const;             // Draws the panel on the right side containing all the buttons and texts
    void reset();                           // Resets the variable like currPieceType, clickedOnRow, and piece selected
    void runSelf(bool mode);                // Provides an option for the engine to play automatically with itself
    void promotionHandler(uint16_t& promo); // this funciton proveds the options to choose which piece to promote to
    void boardInteractionHandler();         // handles all the interaction related to the chess board
    void sidePanelInteractionHandler();     // handles all the interaction related to the right side panel
    void interactionHandler();              // uses the board and sidePanel interaction handler to handles every type of interaction

private:
    size_t sidePanelX_;                     // indicates the points on x axis from where the sidepanel should be drawn
    size_t leftPadding_;                    
    size_t topPadding_;
    std::string logo_;
    Button btnCopyFEN_;
    Button btnEngineToggle_;                // Button to toggle the option of playing against engine
    Button btnFlipBoard_;                   // flips the boards is player wishes to play with black
    Button btnResetBoard_;                  // Resets the boards to initial position
    char currPieceType_;                    // denotes the pieces which is currently held by mouse
    int clickedOnRow_;                      // used to check whether the tile on which mouse was clicked has a piece or not
    int clickedOnCol_;                      // used to check whether the tile on which mouse was clicked has a piece or not
    bool pieceSelected_;                    // Indicate whether a piece is currenty selected via mouse 
};