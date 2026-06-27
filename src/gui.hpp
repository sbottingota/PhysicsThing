#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <optional>

const sf::Font font("times-new-roman.otf");

const sf::Color bg_color = sf::Color::White;
const sf::Color border_color = sf::Color::Black;
const sf::Color text_color = sf::Color::Black;
const sf::Color fg_color(150, 150, 150); // gray
const sf::Color interacted_color(80, 80, 80); // darker gray

const float border_thickness = 2;

class GUI;

class GUIComponent : public sf::Drawable {
    public:
    const float x, y;
    const float width, height;

    GUIComponent(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}

    // to be overwritten in subclasses
    virtual void handle_event(const sf::Event &event) {}
    virtual void update(float dt) {}

    virtual sf::Cursor::Type mouse_type() const { return sf::Cursor::Type::Arrow; }
};

class Button : public GUIComponent {
    constexpr static float animation_time = 0.3; // the time that the "button press" animation takes, in seconds

    std::string text;
    std::function<void()> action;

    float pressed_time = 0;

    public:
    Button(std::string text, float x, float y, float width, float height)
        : GUIComponent(x, y, width, height), text(text) {}

    void set_action(std::function<void()> action);

    virtual void handle_event(const sf::Event &event) override;
    virtual void update(float dt) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual sf::Cursor::Type mouse_type() const { return sf::Cursor::Type::Hand; }
};

class NumberInput : public GUIComponent {
    std::string text = "";
    bool in_focus = false;

    sf::RectangleShape box;
    sf::Text text_object = sf::Text(font);

    public:
    NumberInput(float x, float y, float width, float height);

    std::optional<float> get_number() const;

    virtual void handle_event(const sf::Event &event) override;
    virtual void update(float dt) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual sf::Cursor::Type mouse_type() const { return sf::Cursor::Type::Text; }
};

class Label : public GUIComponent {
    sf::Text text_object = sf::Text(font);

    public:
    Label(std::string text, float x, float y);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class GUI {
    const static sf::Cursor::Type default_cursor_type = sf::Cursor::Type::Arrow;

    sf::RenderWindow gui_window;
    std::vector<std::shared_ptr<GUIComponent>> components;

    std::optional<sf::Cursor> cursor = sf::Cursor::createFromSystem(default_cursor_type);

    public:
    GUI(unsigned int width, unsigned int height, std::string window_name);

    void add_component(std::shared_ptr<GUIComponent> component);

    void update(float dt);

    sf::RenderWindow &get_render_window();
};

#endif
