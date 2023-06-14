#include "Network.hpp"
#include "SFML/Graphics.hpp"
using namespace sf;

int main()
{
    ifstream infile("dataset.dat", ios::binary);
    vector<float> input(9), output(2), res(2);
    vector<int> layers = { 9, 4, 3, 2 };
    Network network(layers, 1);
    network.load();
    float er = 0;
    bool b;
    int p = 0, n = 0;
    do
    {   
        system("cls");
        er = 0; p = 0;
        for (int j = 0; j < 16; j++)
        {
            for (int i = 0; i < 9; i++)
            {
                infile.seekg(p++);
                infile.read((char*)&b, sizeof(bool));
                input[i] = b;
            }
            for (int i = 0; i < 2; i++)
            {
                infile.read((char*)&b, sizeof(bool));
                output[i] = b ? 0.8 : 0.2;
                infile.seekg(++p);
            }
            res = network.signal_passing(input);
            er += network.back_propogation(output);
            for (int i = 0; i < 9; i++) cout << input[i] << " ";
            for (int i = 0; i < 2; i++) cout << output[i] << " ";
            cout << res[0] << " " << res[1] << endl;
        }
        cout << er << " " << n++ << endl << endl;
    } while (er > 1e-6 && n < 30000);
    network.store();
    RenderWindow window(VideoMode(1920, 1080), "Neural network", Style::Close);
    vector<RectangleShape> inputs(9, RectangleShape(Vector2f(200, 200)));
    vector<RectangleShape> outputs(2, RectangleShape(Vector2f(200, 200)));
    Font font;
    font.loadFromFile("arialmt.ttf");
    Text text;
    Text H, V, horout, verout;
    H.setString("hor"); H.setFont(font);  H.setCharacterSize(120); H.setPosition(Vector2f(1230, 220));
    V.setString("ver"); V.setFont(font); V.setCharacterSize(120); V.setPosition(Vector2f(1230, 425));
    horout.setFont(font);  horout.setCharacterSize(80); horout.setPosition(Vector2f(1500, 250));
    verout.setFont(font);  verout.setCharacterSize(80); verout.setPosition(Vector2f(1500, 455));
    float x, y;
    for (int i = 0; i < 9; i++) 
    {
    inputs[i].setFillColor(Color::Black);
    inputs[i].setOutlineColor(Color::White);
    inputs[i].setOutlineThickness(4);
    inputs[i].setPosition(Vector2f(200 + 215 * (i % 3), 200 + 215 * (i / 3)));
    if (i < 2)
    {
        outputs[i].setFillColor(Color::Black);
        outputs[i].setOutlineColor(Color::White);
        outputs[i].setOutlineThickness(4);
        outputs[i].setPosition(Vector2f(1000, 200 + 215 * (i % 3)));
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
            for (int i = 0; i < 9; i++)
                if (x > inputs[i].getPosition().x && x < inputs[i].getPosition().x + 208 &&
                    y > inputs[i].getPosition().y && y < inputs[i].getPosition().y + 208)
                    inputs[i].setFillColor((inputs[i].getFillColor() == Color::White) ? Color::Black : Color::White);
            break;
        default: 
        for (int i = 0; i < 9; i++)
        {
            input[i] = (inputs[i].getFillColor() == Color::White) ? 1 : 0;
        }
        res = network.signal_passing(input);
        horout.setString(to_string(res[0]));
        outputs[0].setFillColor((res[0] >= 0.7 && res[0] <= 0.9) ? Color::White : Color::Black);
        outputs[1].setFillColor((res[1] >= 0.7 && res[1] <= 0.9) ? Color::White : Color::Black);
        verout.setString(to_string(res[1]));
        break;
        }
    }
    window.clear();
    for (int i = 0; i < 9; i++) window.draw(inputs[i]);
    for (int i = 0; i < 2; i++) window.draw(outputs[i]); 
    window.draw(horout);
    window.draw(verout);
    window.draw(text);
    window.draw(H);
    window.draw(V);
    window.display();
    } 
	return 0;
}