#include <cstdint>
#include <string>
#include <functional>
#include "raylib.h"

class Button
{
public:
    Button() = default;

    Button
    (   Rectangle base, Color baseColor, Color borderColor,
        std::string label, size_t labelHeight,
        Color labelColor, 
        std::function<void()> onClick   
    );

    // Utility Functions
    void draw() const;                  // Draws the button on the screen

    // Interacting Functions
    void interactionHandler();          // What to do when user interact with button 

private:
    Rectangle base_;
    Color baseColor_;                   // color of button by default is
    Color borderColor_;                      // color of the rounded edge of the button
    size_t topRightX_;                  // Used to determine whether the mouse is inside the button on x axis
    size_t bottomleftY_;                // Used to determine whether the mouse is inside the button on y axis
    std::string label_;                 // Text written on button
    size_t labelX_;
    size_t labelY_;                     
    size_t labelHeight_;                // size of label in the button
    size_t labelWidth_;                 // total width of label depending on font size and string length                    
    Color labelColor_;                  // color of text by default is dark grey
    std::function<void()> onClick_;     // funtion pointer to take custom implementation of onClick() for each object
};