#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <optional>

#define GUI_WIDTH 300
#define GUI_HEIGHT 720

#define PADDING (GUI_WIDTH / 15)

const sf::Font font("times-new-roman.otf");

const sf::Color bg_color = sf::Color::White;
const sf::Color border_color = sf::Color::Black;
const sf::Color text_color = sf::Color::Black;
const sf::Color fg_color(150, 150, 150); // gray
const sf::Color interacted_color(80, 80, 80); // darker gray

const float border_thickness = 2;

class GUI;

class GUIComponent : public sf::Drawable {
    protected:
    float x, y;
    float width, height;

    public:
    GUIComponent(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

    // to be overwritten in subclasses
    virtual void handle_event(const sf::Event &event) {}
    virtual void update(float dt, GUI &gui) {}
};

class Button : public GUIComponent {
    constexpr static float animation_time = 0.3; // the time that the "button press" animation takes, in seconds

    std::optional<sf::Cursor::Type> cursor_type;

    std::string text;
    std::function<void()> action;

    float pressed_time = 0;

    public:
    Button(std::string text, float x, float y, float width, float height)
        : text(text), GUIComponent(x, y, width, height) {}

    void set_action(std::function<void()> action);

    virtual void handle_event(const sf::Event &event) override;
    virtual void update(float dt, GUI &gui) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class NumberInput : public GUIComponent {
    std::string text = "";
    bool in_focus = false;

    public:
    NumberInput(float x, float y, float width, float height) : GUIComponent(x, y, width, height) {}

    float get_number() const;

    virtual void handle_event(const sf::Event &event) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class GUI {
    sf::RenderWindow gui_window;
    std::vector<std::shared_ptr<GUIComponent>> components;

    public:
    GUI(unsigned int width, unsigned int height, std::string window_name);

    void add_component(std::shared_ptr<GUIComponent> component);

    void update(float dt);

    sf::RenderWindow &get_render_window();
};

GUI create_simulation_gui();

#endif
