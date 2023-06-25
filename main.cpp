#include "Network.hpp"
#include "SFML/Graphics.hpp"
using namespace sf;

const int len = 28;

int reverseInt(int i)
{
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
void read_mnist()
{
    ifstream images("train-images.idx3-ubyte", ios::binary);
    unsigned int magic_number = 0;
    unsigned int number_of_images = 0;
    unsigned int n_rows = 0;
    unsigned int n_cols = 0;
    images.read((char*)&magic_number, sizeof(magic_number));
    magic_number = reverseInt(magic_number);
    images.read((char*)&number_of_images, sizeof(number_of_images));
    number_of_images = reverseInt(number_of_images);
    images.read((char*)&n_rows, sizeof(n_rows));
    n_rows = reverseInt(n_rows);
    images.read((char*)&n_cols, sizeof(n_cols));
    n_cols = reverseInt(n_cols);
    ifstream labels("train-labels.idx1-ubyte", ios::binary);
    images.seekg(len * len + 16);
    labels.seekg(9);
    ofstream dataset("dataset.dat", ios::binary);
    for (int i = 0; i < number_of_images - 1; ++i)
    {
        for (int r = 0; r < n_rows; ++r)
        {
            for (int c = 0; c < n_cols; ++c)
            {
                unsigned char temp = 0;
                images.read((char*)&temp, sizeof(temp));
                if (images)
                {
                    bool b = temp;
                    dataset.write((char*)&b, 1);
                }
                else
                {
                    bool b = 0;
                    dataset.write((char*)&b, 1);
                    images.close();
                    images.open("dataset.dat", ios::binary);
                    images.seekg(i * 785 + r * len + c);
                }
            }
        }
        unsigned char c;
        labels.read((char*)&c, 1);
        cout << (int)c << " " << i << endl;
        dataset.write((char*)&c, 1);
    }
}

int main()
{
    const int size = 30;
    vector<float> input(len * len), output(10), res(10);
    vector<int> layers = { len * len, 300, 50, 10 };
    Network network(layers, 1);
    float er = 0, tot_er = 0, x, y;
    bool b;
    int p = 0, s = 0, n = 0;

    RenderWindow window(VideoMode(1920, 1080), "Neural network", Style::Close);

    RectangleShape frame(Vector2f(860, 860));
    frame.setFillColor(Color(127, 127, 127));
    frame.setPosition(Vector2f(90, 90));

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

    vector<Text> digits(10);
    vector<Text> result(10);
    for (int i = 0; i < 10; i++)
    {
        digits[i].setFont(font);
        digits[i].setString(to_string(i));
        digits[i].setCharacterSize(70);
        digits[i].setPosition(Vector2f(1070, 85 + 70 * (i % 10)));
        digits[i].setFillColor(Color::White);
        result[i].setFont(font);
        result[i].setString("N/A");
        result[i].setCharacterSize(70);
        result[i].setPosition(Vector2f(1210, 85 + 70 * (i % 10)));
        result[i].setFillColor(Color::White);
    }

    vector<RectangleShape> inputs(len * len, RectangleShape(Vector2f(size, size)));
    vector<RectangleShape> outputs(10, RectangleShape(Vector2f(50, 50)));
    vector<RectangleShape> net_outputs(10, RectangleShape(Vector2f(50, 50)));

    Text steps;
    steps.setFont(font);
    steps.setString("Steps");
    steps.setCharacterSize(50);
    steps.setPosition(Vector2f(1000, 800));
    steps.setFillColor(Color::White);

    Text s_steps;
    s_steps.setFont(font);
    s_steps.setString(to_string(s));
    s_steps.setCharacterSize(40);
    s_steps.setPosition(Vector2f(1000, 870));
    s_steps.setFillColor(Color::White);

    Text n_steps;
    n_steps.setFont(font);
    n_steps.setString(to_string(n));
    n_steps.setCharacterSize(40);
    n_steps.setPosition(Vector2f(1070, 870));
    n_steps.setFillColor(Color::White);
    
    Text Error;
    Error.setFont(font);
    Error.setString("Error");
    Error.setCharacterSize(50);
    Error.setPosition(Vector2f(1200, 800));
    Error.setFillColor(Color::White);

    Text t_error;
    t_error.setFont(font);
    t_error.setString(to_string(tot_er));
    t_error.setCharacterSize(40);
    t_error.setPosition(Vector2f(1200, 870));
    t_error.setFillColor(Color::White);

    ifstream ifile("dataset.dat", ios::binary);

    ofstream error_file("error.txt");

    for (int i = 0; i < len * len; i++)
    {
        inputs[i].setPosition(Vector2f(100 + size * (i % len), 100 + size * (i / len)));
        if (i < 10)
        {
            outputs[i].setFillColor(Color::Black);
            outputs[i].setOutlineColor(Color::White);
            outputs[i].setOutlineThickness(4);
            outputs[i].setPosition(Vector2f(1000, 105 + 70 * (i % 10)));
            net_outputs[i].setFillColor(Color::Black);
            net_outputs[i].setOutlineColor(Color::White);
            net_outputs[i].setOutlineThickness(4);
            net_outputs[i].setPosition(Vector2f(1130, 105 + 70 * (i % 10)));
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed: window.close(); ifile.close(); break;
           /* case Event::MouseButtonPressed:
                x = event.mouseButton.x;
                y = event.mouseButton.y;
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
                break;*/
            }
        }
        for (; n < 59999; n++)
        {
            for (int i = 0; i < len * len; i++)
            {
                bool b;
                ifile.read((char*)&b, 1);
                input[i] = b;
                inputs[i].setFillColor(b ? Color::White : Color::Black);
                if (i < 10)
                {
                outputs[i].setFillColor(Color::Black);
                net_outputs[i].setFillColor(Color::Black);
                }
            }

            char c;
            ifile.read((char*)&c, 1);
            outputs[c].setFillColor(Color::White);
            for (int i = 0; i < 10; i++) output[i] = 0;
            output[c] = 1;

            res = network.signal_passing(input);
            er += network.back_propogation(output);

            float max = 0;
            int index = 0;
            for (int i = 0; i < 10; i++)
            {
                result[i].setString(to_string(res[i]));
                if (res[i] > max)
                {
                    max = res[i];
                    index = i;
                }
            }
            net_outputs[index].setFillColor(Color::White);
            n_steps.setString(to_string(n));
            window.clear();
            window.draw(frame);
            window.draw(steps);
            window.draw(s_steps);
            window.draw(n_steps);
            window.draw(Error);
            window.draw(t_error);
            //window.draw(clear);
            //window.draw(tclear);
            for (int i = 0; i < len * len; i++) window.draw(inputs[i]);
            for (int i = 0; i < 10; i++)
            {
                window.draw(outputs[i]);
                window.draw(net_outputs[i]);
                window.draw(digits[i]);
                window.draw(result[i]);
            }
            window.display();
        }
        ifile.seekg(0);
        n = 0;
        tot_er = er;
        er = 0;
        t_error.setString(to_string(tot_er));
        s++;
        s_steps.setString(to_string(s));
        network.store();
        error_file << tot_er << "\n";
    }
    return 0;
}
