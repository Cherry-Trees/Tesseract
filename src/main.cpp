/** REFERENCES
 *  ->  https://mathematica.stackexchange.com/questions/9580/how-to-create-this-four-dimensional-cube-animation
 *  ->  https://luicat.github.io/2018/05/23/4D-rendering.html
 *  ->  https://math.stackexchange.com/questions/1402362/can-rotations-in-4d-be-given-an-explicit-matrix-form
*/

#include <SFML/Graphics.hpp>
#include "Tesseract.h"

int main() {

    // Window size
    const unsigned window_width = 1200;
    const unsigned window_height = 1200;

    // Rotation speed
    const float tick_speed = 0.0001f;

    // Witchcraft to efficiently calculate sin and cos
    const float sin_tick = tick_speed;
    const float cos_tick = 1;
    const float sin_tick2 = 2 * sin_tick;
    const float cos_tick2 = 1;

    // Vector transformation matrices
    const Mat4 rotationXZ = mat4RotationXZ(sin_tick2, cos_tick2);
    const Mat4 rotationYW = mat4RotationYW(sin_tick, cos_tick);
    const Mat4 projection = mat4Projection(window_width, window_height);

    Tesseract tesseract = make_tesseract();
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Tesseract");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        Vec4 projectedVertices[16];

        // Rotate and project vertices
        for (int i = 0; i < 16; i++) {
            
            // Rotate
            tesseract.vertices[i] = multiply(tesseract.vertices[i], rotationXZ);
            tesseract.vertices[i] = multiply(tesseract.vertices[i], rotationYW);

            // Make a copy
            projectedVertices[i] = tesseract.vertices[i];
            
            // Map the w component onto 3D space
            projectedVertices[i] = mapW(projectedVertices[i]);

            // Translate into camera view
            projectedVertices[i].z += 4;

            // Project onto 2D space
            projectedVertices[i] = multiply(projectedVertices[i], projection);
            projectedVertices[i] = normalizeW(projectedVertices[i]);

            // Translate into window view
            projectedVertices[i].x += 1;
            projectedVertices[i].y += 1;

            // Scale to window size
            projectedVertices[i].x *= 0.5 * window_width;
            projectedVertices[i].y *= 0.5 * window_height;
        }

        // Draw the wireframe
        for (auto &&edge : tesseract.edges) {
            Vec4 projectedVertex1 = projectedVertices[edge.u];
            Vec4 projectedVertex2 = projectedVertices[edge.v];

            sf::Vector2f sfProjectedVertex1 = { projectedVertex1.x, projectedVertex1.y };
            sf::Vector2f sfProjectedVertex2 = { projectedVertex2.x, projectedVertex2.y };

            sf::Vertex line[2] = { sfProjectedVertex1, sfProjectedVertex2 };
            window.draw(line, 2, sf::Lines);
        }

        // Fill in the wireframe
        for (auto &&quad : tesseract.quads) {

            Vec4 projectedVertex1 = projectedVertices[quad.u];
            Vec4 projectedVertex2 = projectedVertices[quad.v];
            Vec4 projectedVertex3 = projectedVertices[quad.p];
            Vec4 projectedVertex4 = projectedVertices[quad.q];

            sf::Vector2f sfProjectedVertex1 = { projectedVertex1.x, projectedVertex1.y };
            sf::Vector2f sfProjectedVertex2 = { projectedVertex2.x, projectedVertex2.y };
            sf::Vector2f sfProjectedVertex3 = { projectedVertex3.x, projectedVertex3.y };
            sf::Vector2f sfProjectedVertex4 = { projectedVertex4.x, projectedVertex4.y };

            sf::ConvexShape convex;

            convex.setPointCount(4);
            convex.setFillColor(sf::Color(200, 200, 255, 50));

            convex.setPoint(0, sfProjectedVertex1);
            convex.setPoint(1, sfProjectedVertex2);
            convex.setPoint(2, sfProjectedVertex3);
            convex.setPoint(3, sfProjectedVertex4);
            window.draw(convex);
        }

        window.display();
    }

    return 0;
}


// TO RUN:

// g++ -I"C:\SFML-2.6.1\include" -c main.cpp -o main.o
// g++ main.o -o app.exe -L"C:\SFML-2.6.1\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

// OR

// MinGW32-make compile
// MinGW32-make link

// OR

// MinGW32-make all
