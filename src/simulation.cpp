#include "simulation.hpp"

GUI create_simulation_gui(float width, Group &simulation_group) {
    const float height = width * 10;

    GUI gui(width, height, "Physics Thing GUI");

    auto add_circle_label = std::make_shared<Label>("Add Circle:");
    gui.add_row(GUIRow({add_circle_label}));

    auto x_label = std::make_shared<Label>("X:");
    auto x_input = std::make_shared<NumberInput>();

    auto y_label = std::make_shared<Label>("Y:");
    auto y_input = std::make_shared<NumberInput>();

    gui.add_row(GUIRow({x_label, x_input, y_label, y_input}));

    auto button = std::make_shared<Button>("Add Circle");
    button->set_action([x_input, y_input, &simulation_group]() mutable {
        std::optional<float> x = x_input->get_number();
        std::optional<float> y = y_input->get_number();

        if (x.has_value() && y.has_value()) {
            auto circle = std::make_shared<Circle>(Pos2(x.value(), y.value()), Vec2(0, 0), 10, 50);
            simulation_group.add_object(circle);
        }
    });
    gui.add_row(GUIRow({button}));

    return gui;
}
