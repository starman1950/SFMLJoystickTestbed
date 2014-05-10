#include <stdio.h>
#include <sstream>

#include "Button.h"
#include "ResourcePath.h"
#include "View.h"

///////////////////////////////////////////////////////////
View::View(unsigned int joystickIndex)
{
    this->joystickIndex = joystickIndex;
    font.loadFromFile(resourcePath("ClearSans-Regular.ttf"));

    sf::Joystick::Identification id = sf::Joystick::getIdentification(joystickIndex);

    {
        name = sf::Text(id.name, font, 36);
        sf::FloatRect bounds = name.getGlobalBounds();
        name.setPosition((1024.0f - bounds.width) / 2.0f, 0);
    }

    {
        std::ostringstream stream;
        stream << "Vendor ID: " << id.vendorId << "    Product ID: " << id.productId;
        vendorAndProduct = sf::Text(stream.str(), font, 24);
        sf::FloatRect bounds = vendorAndProduct.getGlobalBounds();
        vendorAndProduct.setPosition((1024.0f - bounds.width) / 2.0f, 46);
    }

    {
        titleSeparator = sf::RectangleShape(sf::Vector2f(1024.0f, 2.0f));
        titleSeparator.setPosition(0, 86);
    }

    {
        buttonHeader = sf::Text("Buttons", font, 36);
        buttonHeader.setPosition(64, 120);
        unsigned int cols = 0;
        float width = 192.0f;
        float height = 64.0f;
        float y = 200.0f;
        for (unsigned int i = 0; i < sf::Joystick::getButtonCount(joystickIndex); ++i)
        {
            float x = cols * width + 64;
            float buttonWidth = 30.0f;
            Button btn(x, y);
            buttons.push_back(btn);

            std::ostringstream stream;
            stream << i;
            sf::Text label(stream.str(), font, 26);
            label.setPosition(x + buttonWidth + 10, y - 6);
            buttonLabels.push_back(label);

            cols++;
            if (cols >= 5)
            {
                cols = 0;
                y += height;
            }
        }
    }

    {
        buttonSeparator = sf::RectangleShape(sf::Vector2f(1024.0f, 2.0f));
        buttonSeparator.setPosition(0, 384);
    }

    {
        axesHeader = sf::Text("Axes", font, 36);
        axesHeader.setPosition(64, 404);
    }

    {
        unsigned int cols = 0;
        float width = 320.0f;
        float height = 64.0f;
        float y = 460.0f;
        for (int i = sf::Joystick::X; i <= sf::Joystick::PovY; ++i)
        {
            float x = cols * width + 64;
            sf::Text axisText = sf::Text("", font, 26);
            sf::Joystick::Axis axis = static_cast<sf::Joystick::Axis>(i);
            if (sf::Joystick::hasAxis(joystickIndex, axis))
            {
                axisText.setString(axisToString(axis));
            }
            else
            {
                axisText.setString(axisToString(axis) + ": N/A");
            }
            axisText.setPosition(x, y);
            axes.insert(std::make_pair(axis, axisText));

            cols++;
            if (cols >= 3)
            {
                cols = 0;
                y += height;
            }
        }
    }

    {
        metaSeparator = sf::RectangleShape(sf::Vector2f(1024.0f, 2.0f));
        metaSeparator.setPosition(0, 640);
    }


}


///////////////////////////////////////////////////////////
View::~View()
{
}


///////////////////////////////////////////////////////////
void View::update()
{
    for (unsigned int i = 0; i < sf::Joystick::getButtonCount(joystickIndex); ++i)
    {
        if (sf::Joystick::isButtonPressed(joystickIndex, i))
        {
            buttons.at(i).setDown();
        }
        else
        {
            buttons.at(i).setUp();
        }
    }

    for (int i = sf::Joystick::X; i <= sf::Joystick::PovY; ++i)
    {
        sf::Joystick::Axis axis = static_cast<sf::Joystick::Axis>(i);
        if (sf::Joystick::hasAxis(joystickIndex, axis))
        {
            std::ostringstream stream;
            stream << sf::Joystick::getAxisPosition(joystickIndex, axis);
            axes[axis].setString(axisToString(axis) + ": " + stream.str());
        }
        else
        {
            axes[axis].setString(axisToString(axis) + ": N/A");
        }
    }
}


///////////////////////////////////////////////////////////
void View::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(name);
    target.draw(vendorAndProduct);
    target.draw(titleSeparator);


    target.draw(buttonHeader);
    for (unsigned int i = 0; i < buttons.size(); ++i)
    {
        target.draw(buttons.at(i));
    }

    for (unsigned int i = 0; i < buttonLabels.size(); ++i)
    {
        target.draw(buttonLabels.at(i));
    }
    target.draw(buttonSeparator);


    target.draw(axesHeader);
    for (AxisMap::const_iterator it = axes.begin(); it != axes.end(); ++it)
    {
        target.draw(it->second);
    }


    target.draw(metaSeparator);

}


/////////////////////////////////////////////////////////////
std::string View::axisToString(sf::Joystick::Axis axis)
{
    switch (axis)
    {
    case sf::Joystick::X:
        return std::string("X");

    case sf::Joystick::Y:
        return std::string("Y");

    case sf::Joystick::Z:
        return std::string("Z");

    case sf::Joystick::U:
        return std::string("U");

    case sf::Joystick::V:
        return std::string("V");

    case sf::Joystick::R:
        return std::string("R");

    case sf::Joystick::PovX:
        return std::string("PovX");

    case sf::Joystick::PovY:
        return std::string("PovY");

    default:
        return std::string("Unknown");
    }

}