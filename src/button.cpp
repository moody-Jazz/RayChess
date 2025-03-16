#include "button.hpp"
#include "global.hpp"

Button::Button
(   
    Rectangle base, Color baseColor, Color borderColor,
    std::string label, size_t labelHeight,
    Color labelColor, 
    std::function<void()> onClick
):  
    // Setting up the members according the parameter using initializer list
    base_(base), baseColor_(baseColor), borderColor_(borderColor),

    topRightX_(base.x + base.width), bottomleftY_(base.y + base.height),
    label_(label), labelHeight_(labelHeight),

    labelWidth_(MeasureText(label.c_str(), labelHeight)),

    labelColor_(labelColor),
    onClick_(onClick)
{
    labelX_ = base.x + (base.width - labelWidth_) / 2;
    labelY_ = base.y + (base.height - labelHeight_) / 2;
}

void Button::draw() const
{
    DrawRectangleRounded(base_, 0.2, 20, baseColor_);
    DrawRectangleRoundedLines(base_, 0.2, Globals::btnHeight/2, borderColor_);
    DrawText(label_.c_str(),labelX_, labelY_, labelHeight_, labelColor_);
}

void Button::interactionHandler()
{
    Vector2 mousePosition = GetMousePosition();
    bool isMouseInsideButton = // check if mouse hovering onto the button
        (mousePosition.x > base_.x && mousePosition.x < topRightX_) &&
        (mousePosition.y > base_.y && mousePosition.y < bottomleftY_);

    // if button is bieng clicked
    if(isMouseInsideButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Darken the button color and make a sound to highlight that it has been clicked
        Globals::sound.playButtonClicked();
        DrawRectangleRounded(base_, 0.2, 20, labelColor_);

        // Call the custom implimentation of this button onClick()
        onClick_();
    }
}

void Button::setUIparams( 
    Rectangle base, Color baseColor, Color borderColor,
    std::string label, size_t labelHeight,
    Color labelColor
)
{
    base_           = base;
    baseColor_      = baseColor;
    borderColor_    = borderColor;
    topRightX_      = base.x + base.width; 
    bottomleftY_    = base.y + base.height;
    label_          = label; 
    labelHeight_    = labelHeight;
    labelWidth_     = MeasureText(label.c_str(), labelHeight);
    labelColor_     = labelColor;
    labelX_         = base.x + (base.width - labelWidth_) / 2;
    labelY_         = base.y + (base.height - labelHeight_) / 2;
}