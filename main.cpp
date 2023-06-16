#include "Network.hpp"
#include "SFML/Graphics.hpp"
using namespace sf;


int main()
{
    const int size = 30, len = 25;
    vector<float> input(9), output(2), res(2);
    vector<int> layers = { 9, 4, 3, 2 };
    Network network(layers, 1);
    network.load();
    float er = 0;
    bool b;
    int p = 0, n = 0;
    RenderWindow window(VideoMode(1920, 1080), "Neural network", Style::Close);
    RectangleShape frame(Vector2f(770, 770));
    frame.setFillColor(Color(127, 127, 127));
    frame.setPosition(Vector2f(190, 190));
    RectangleShape clear(Vector2f(200, 100));
    clear.setPosition(Vector2f(500, 50));
    clear.setFillColor(Color::Black);
    clear.setOutlineColor(Color::White);
    clear.setOutlineThickness(3);
    Font font;
    font.loadFromFile("arialmt.ttf");
    Text tclear;
    tclear.setFont(font);
    tclear.setString("Clear");
    tclear.setCharacterSize(80);
    tclear.setPosition(Vector2f(505, 45));
    tclear.setFillColor(Color::White);
    vector<RectangleShape> inputs(len * len, RectangleShape(Vector2f(size, size)));
    vector<RectangleShape> outputs(11, RectangleShape(Vector2f(50, 50)));
    float x, y;
    for (int i = 0; i < len * len; i++)
    {
        inputs[i].setFillColor(Color::Black);
        inputs[i].setPosition(Vector2f(200 + size * (i % len), 200 + size * (i / len)));
        if (i < 11)
        {
            outputs[i].setFillColor(Color::Black);
            outputs[i].setOutlineColor(Color::White);
            outputs[i].setOutlineThickness(4);
            outputs[i].setPosition(Vector2f(1000, 205 + 70 * (i % 11)));
        }
    }
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed: window.close(); break;
            case Event::MouseButtonPressed:
                x = event.mouseButton.x;
                y = event.mouseButton.y;
                for (int i = 0; i < 11; i++)
                    if (x > outputs[i].getPosition().x && x < outputs[i].getPosition().x + 50 &&
                        y > outputs[i].getPosition().y && y < outputs[i].getPosition().y + 50)
                        outputs[i].setFillColor((outputs[i].getFillColor() == Color::White) ? Color::Black : Color::White);
                if (x > clear.getPosition().x && x < clear.getPosition().x + 200 &&
                    y > clear.getPosition().y && y < clear.getPosition().y + 100)
                    for (int i = 0; i < len * len; i++)
                        inputs[i].setFillColor(Color::Black);
                break;
            default:
                float x, y;
                x = Mouse::getPosition().x - window.getPosition().x - 9;
                y = Mouse::getPosition().y - window.getPosition().y - 38;
                for (int i = 0; i < len * len; i++)
                {
                    if (x > inputs[i].getPosition().x && x < inputs[i].getPosition().x + size &&
                        y > inputs[i].getPosition().y && y < inputs[i].getPosition().y + size)
                        if (Mouse::isButtonPressed(Mouse::Left)) inputs[i].setFillColor(Color::White);
                        else if (Mouse::isButtonPressed(Mouse::Right)) inputs[i].setFillColor(Color::Black);
                }
                break;
            }
        }
        window.clear();
        window.draw(frame);
        window.draw(clear);
        window.draw(tclear);
        for (int i = 0; i < len * len; i++) window.draw(inputs[i]);
        for (int i = 0; i < 11; i++) window.draw(outputs[i]);
        window.display();
    }
    return 0;
}
