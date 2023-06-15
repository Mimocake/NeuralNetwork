#include "Network.hpp"
#include "SFML/Graphics.hpp"
using namespace sf;

struct pnt
{
    float x, y;
    Color color;
    CircleShape circ;
    pnt() {}
    pnt(float xx, float yy, Color col) : x(xx), y(yy), color(col)
    {
        circ.setPosition(Vector2f(x, y));
        circ.setRadius(7);
        circ.setOutlineColor(Color::Black);
        circ.setOutlineThickness(2);
        circ.setFillColor(col);
    }
};

int main()
{
    const int height = 1280, width = 720, size = 20;
    ifstream infile("dataset.dat", ios::binary);
    vector<float> input(9), output(2), res(2);
    vector<int> layers = { 2, 5, 5, 2 };
    Network network(layers, 1);
    RenderWindow window(VideoMode(height, width), "Neural network", Style::Close);
    vector<pnt> points(0);
    vector<vector<RectangleShape>> screen(height / size);
    for (int i = 0; i < width / size; i++)
    {
        screen[i] = vector<RectangleShape>(height / size, RectangleShape(Vector2f(size, size)));
        for (int j = 0; j < height / size; j++)
        {
            screen[i][j].setFillColor(Color(0, 255, 255));
            screen[i][j].setPosition(Vector2f(j * size, i * size));
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
                if (event.mouseButton.button == Mouse::Left)
                    points.push_back(pnt(event.mouseButton.x, event.mouseButton.y, Color::Green));
                else points.push_back(pnt(event.mouseButton.x, event.mouseButton.y, Color::Blue));
                break;
            default: 
                for (int i = 0; i < points.size(); i++)
                {
                    float x = points[i].x / (0.5 * height) - 1;
                    float y = points[i].y / (0.5 * width) - 1;
                    res = network.signal_passing(vector<float>{x, y});
                    cout << res[0] << " " << res[1] << endl;
                    network.back_propogation(vector<float>{(float)points[i].color.g / 255, (float)points[i].color.b / 255});
                }
                if (points.size())
                {
                    for (int i = 0; i < width / size; i++)
                        for (int j = 0; j < height / size; j++)
                        {
                            Vector2f vec = screen[i][j].getPosition();
                            float x = (vec.x) / (0.5 * height) - 1;
                            float y = (vec.y) / (0.5 * width) - 1;
                            res = network.signal_passing(vector<float>{x, y});
                            screen[i][j].setFillColor(Color(50, res[0] * 255, res[1] * 255));
                        }
                }
            break;
            }
        }
        for (int i = 0; i < width / size; i++)
            for (int j = 0; j < height / size; j++)
                window.draw(screen[i][j]);
        for (int i = 0; i < points.size(); i++)
            window.draw(points[i].circ);
        window.display();
    } 
	return 0;
}