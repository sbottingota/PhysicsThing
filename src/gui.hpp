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

const sf::Cursor::Type default_cursor_type = sf::Cursor::Type::Arrow;

const float border_thickness = 2;

const float font_size_multiplier = 0.9; // proportion of the height of an object that text should take up

struct Bounds {
    float x, y;
    float width, height;

    Bounds() : x(0), y(0), width(0), height(0) {}
    Bounds(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {}
};


class GUIComponent : public sf::Drawable {
    protected:
    Bounds bounds;

    public:
    void set_bounds(Bounds bounds);

    // to be overwritten in subclasses
    virtual void handle_event(const sf::Event &event) {}
    virtual void update(float dt) {}

    virtual sf::Cursor::Type mouse_type() const { return sf::Cursor::Type::Arrow; }

    float x() const { return bounds.x; }
    float y() const { return bounds.y; }
    float width() const { return bounds.width; }
    float height() const { return bounds.height; }
};

class Button : public GUIComponent {
    constexpr static float animation_time = 0.3; // the time that the "button press" animation takes, in seconds

    std::string text;
    std::function<void()> action;

    float pressed_time = 0;

    public:
    Button(std::string text) : text(text) {}

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
    NumberInput() {}

    std::optional<float> get_number() const;

    virtual void update(float dt) override;
    virtual void handle_event(const sf::Event &event) override;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    virtual sf::Cursor::Type mouse_type() const { return sf::Cursor::Type::Text; }
};

class Label : public GUIComponent {
    std::string text;

    public:
    Label(std::string text) : text(text) {}

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class GUIRow : public sf::Drawable {
    static constexpr float padding_proportion = 0.1; // the portion of the width that each area of padding takes up

    std::vector<std::shared_ptr<GUIComponent>> components;

    Bounds bounds;

    void update_component_bounds() const;

    public:
    GUIRow(size_t n_components);
    GUIRow(std::vector<std::shared_ptr<GUIComponent>> components) : components(components) {}


    void add_component(std::shared_ptr<GUIComponent> component, size_t idx);
    void set_bounds(Bounds bounds);

    // these just pass the method call onto the respective components
    void update(float dt);
    void handle_event(const sf::Event &event);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::optional<sf::Cursor::Type> mouse_type(float mouse_x, float mouse_y) const;
};

class GUI {
    sf::RenderWindow gui_window;
    std::vector<GUIRow> rows;

    std::optional<sf::Cursor> cursor = sf::Cursor::createFromSystem(default_cursor_type);

    public:
    GUI(unsigned int width, unsigned int height, std::string window_name);

    void add_row(GUIRow row);

    void update(float dt);

    sf::RenderWindow &get_render_window();
};

#endif
