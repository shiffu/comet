#include "glfwInput.h"
#include <comet/application.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <comet/platform.h>

namespace comet
{

    #if COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_GLFW
        Input* Input::s_instance = new GLFWInput();
    #endif


    std::unordered_map<Input::MouseButton, int> GLFWInput::m_cometButtonToGLFWButtonMap ({
        {MouseButton::Left,        GLFW_MOUSE_BUTTON_LEFT},
        {MouseButton::Right,       GLFW_MOUSE_BUTTON_RIGHT},
        {MouseButton::Middle,      GLFW_MOUSE_BUTTON_MIDDLE},
        {MouseButton::XButton1,    GLFW_MOUSE_BUTTON_4},
        {MouseButton::XButton2,    GLFW_MOUSE_BUTTON_5},
    });

    std::unordered_map<int, Input::MouseButton> GLFWInput::m_GLFWButtonToCometButtonMap ({
        {GLFW_MOUSE_BUTTON_LEFT,    MouseButton::Left},
        {GLFW_MOUSE_BUTTON_RIGHT,   MouseButton::Right},
        {GLFW_MOUSE_BUTTON_MIDDLE,  MouseButton::Middle},
        {GLFW_MOUSE_BUTTON_4,       MouseButton::XButton1},
        {GLFW_MOUSE_BUTTON_5,       MouseButton::XButton2},
    });

    std::unordered_map<Input::Key, int> GLFWInput::m_cometKeyToGLFWKeyMap ({
        {Key::Unknown,          GLFW_KEY_UNKNOWN},
        {Key::Space,            GLFW_KEY_SPACE},
        {Key::Quote,            GLFW_KEY_APOSTROPHE},
        {Key::Comma,            GLFW_KEY_COMMA},
        {Key::Hyphen,           GLFW_KEY_MINUS},
        {Key::Period,           GLFW_KEY_PERIOD},
        {Key::Slash,            GLFW_KEY_SLASH},
        {Key::Num0,             GLFW_KEY_0},
        {Key::Num1,             GLFW_KEY_1},
        {Key::Num2,             GLFW_KEY_2},
        {Key::Num3,             GLFW_KEY_3},
        {Key::Num4,             GLFW_KEY_4},
        {Key::Num5,             GLFW_KEY_5},
        {Key::Num6,             GLFW_KEY_6},
        {Key::Num7,             GLFW_KEY_7},
        {Key::Num8,             GLFW_KEY_8},
        {Key::Num9,             GLFW_KEY_9},
        {Key::Semicolon,        GLFW_KEY_SEMICOLON},
        {Key::Equal,            GLFW_KEY_EQUAL},
        {Key::A,                GLFW_KEY_A},
        {Key::B,                GLFW_KEY_B},
        {Key::C,                GLFW_KEY_C},
        {Key::D,                GLFW_KEY_D},
        {Key::E,                GLFW_KEY_E},
        {Key::F,                GLFW_KEY_F},
        {Key::G,                GLFW_KEY_G},
        {Key::H,                GLFW_KEY_H},
        {Key::I,                GLFW_KEY_I},
        {Key::J,                GLFW_KEY_J},
        {Key::K,                GLFW_KEY_K},
        {Key::L,                GLFW_KEY_L},
        {Key::M,                GLFW_KEY_M},
        {Key::N,                GLFW_KEY_N},
        {Key::O,                GLFW_KEY_O},
        {Key::P,                GLFW_KEY_P},
        {Key::Q,                GLFW_KEY_Q},
        {Key::R,                GLFW_KEY_R},
        {Key::S,                GLFW_KEY_S},
        {Key::T,                GLFW_KEY_T},
        {Key::U,                GLFW_KEY_U},
        {Key::V,                GLFW_KEY_V},
        {Key::W,                GLFW_KEY_W},
        {Key::X,                GLFW_KEY_X},
        {Key::Y,                GLFW_KEY_Y},
        {Key::Z,                GLFW_KEY_Z},
        {Key::LBracket,         GLFW_KEY_LEFT_BRACKET},
        {Key::Backslash,        GLFW_KEY_BACKSLASH},
        {Key::RBracket,         GLFW_KEY_RIGHT_BRACKET},
        {Key::BackTick,         GLFW_KEY_GRAVE_ACCENT},
        {Key::Escape,           GLFW_KEY_ESCAPE},
        {Key::Enter,            GLFW_KEY_ENTER},
        {Key::Tab,              GLFW_KEY_TAB},
        {Key::Backspace,        GLFW_KEY_BACKSPACE},
        {Key::Insert,           GLFW_KEY_INSERT},
        {Key::Delete,           GLFW_KEY_DELETE},
        {Key::Right,            GLFW_KEY_RIGHT},
        {Key::Left,             GLFW_KEY_LEFT},
        {Key::Down,             GLFW_KEY_DOWN},
        {Key::Up,               GLFW_KEY_UP},
        {Key::PageUp,           GLFW_KEY_PAGE_UP},
        {Key::PageDown,         GLFW_KEY_PAGE_DOWN},
        {Key::Home,             GLFW_KEY_HOME},
        {Key::End,              GLFW_KEY_END},
        {Key::Caps,             GLFW_KEY_CAPS_LOCK},
        {Key::ScrollLock,       GLFW_KEY_SCROLL_LOCK},
        {Key::NumLock,          GLFW_KEY_NUM_LOCK},
        {Key::PrintScreen,      GLFW_KEY_PRINT_SCREEN},
        {Key::Pause,            GLFW_KEY_PAUSE},
        {Key::F1,               GLFW_KEY_F1},
        {Key::F2,               GLFW_KEY_F2},
        {Key::F3,               GLFW_KEY_F3},
        {Key::F4,               GLFW_KEY_F4},
        {Key::F5,               GLFW_KEY_F5},
        {Key::F6,               GLFW_KEY_F6},
        {Key::F7,               GLFW_KEY_F7},
        {Key::F8,               GLFW_KEY_F8},
        {Key::F9,               GLFW_KEY_F9},
        {Key::F10,              GLFW_KEY_F10},
        {Key::F11,              GLFW_KEY_F11},
        {Key::F12,              GLFW_KEY_F12},
        {Key::F13,              GLFW_KEY_F13},
        {Key::F14,              GLFW_KEY_F14},
        {Key::F15,              GLFW_KEY_F15},
        {Key::F16,              GLFW_KEY_F16},
        {Key::F17,              GLFW_KEY_F17},
        {Key::F18,              GLFW_KEY_F18},
        {Key::F19,              GLFW_KEY_F19},
        {Key::F20,              GLFW_KEY_F20},
        {Key::F21,              GLFW_KEY_F21},
        {Key::F22,              GLFW_KEY_F22},
        {Key::F23,              GLFW_KEY_F23},
        {Key::F24,              GLFW_KEY_F24},
        {Key::F25,              GLFW_KEY_F25},
        {Key::KP0,              GLFW_KEY_KP_0},
        {Key::KP1,              GLFW_KEY_KP_1},
        {Key::KP2,              GLFW_KEY_KP_2},
        {Key::KP3,              GLFW_KEY_KP_3},
        {Key::KP4,              GLFW_KEY_KP_4},
        {Key::KP5,              GLFW_KEY_KP_5},
        {Key::KP6,              GLFW_KEY_KP_6},
        {Key::KP7,              GLFW_KEY_KP_7},
        {Key::KP8,              GLFW_KEY_KP_8},
        {Key::KP9,              GLFW_KEY_KP_9},
        {Key::KPPeriod,         GLFW_KEY_KP_DECIMAL},
        {Key::KPDivide,         GLFW_KEY_KP_DIVIDE},
        {Key::KPMultiply,       GLFW_KEY_KP_MULTIPLY},
        {Key::KPSubstract,      GLFW_KEY_KP_SUBTRACT},
        {Key::KPAdd,            GLFW_KEY_KP_ADD},
        {Key::KPEnter,          GLFW_KEY_KP_ENTER},
        {Key::KPEqual,          GLFW_KEY_KP_EQUAL},
        {Key::LShift,           GLFW_KEY_LEFT_SHIFT},
        {Key::LControl,         GLFW_KEY_LEFT_CONTROL},
        {Key::LAlt,             GLFW_KEY_LEFT_ALT},
        {Key::LSystem,          GLFW_KEY_LEFT_SUPER},
        {Key::RShift,           GLFW_KEY_RIGHT_SHIFT},
        {Key::RControl,         GLFW_KEY_RIGHT_CONTROL},
        {Key::RAlt,             GLFW_KEY_RIGHT_ALT},
        {Key::RSystem,          GLFW_KEY_RIGHT_SUPER},
        {Key::Menu,             GLFW_KEY_MENU}
    });

    std::unordered_map<int, Input::Key> GLFWInput::m_GLFWKeyToCometKeyMap ({
        {GLFW_KEY_UNKNOWN,          Key::Unknown},
        {GLFW_KEY_SPACE,            Key::Space},
        {GLFW_KEY_APOSTROPHE,       Key::Quote},
        {GLFW_KEY_COMMA,            Key::Comma},
        {GLFW_KEY_MINUS,            Key::Hyphen},
        {GLFW_KEY_PERIOD,           Key::Period},
        {GLFW_KEY_SLASH,            Key::Slash},
        {GLFW_KEY_0,                Key::Num0},
        {GLFW_KEY_1,                Key::Num1},
        {GLFW_KEY_2,                Key::Num2},
        {GLFW_KEY_3,                Key::Num3},
        {GLFW_KEY_4,                Key::Num4},
        {GLFW_KEY_5,                Key::Num5},
        {GLFW_KEY_6,                Key::Num6},
        {GLFW_KEY_7,                Key::Num7},
        {GLFW_KEY_8,                Key::Num8},
        {GLFW_KEY_9,                Key::Num9},
        {GLFW_KEY_SEMICOLON,        Key::Semicolon},
        {GLFW_KEY_EQUAL,            Key::Equal},
        {GLFW_KEY_A,                Key::A},
        {GLFW_KEY_B,                Key::B},
        {GLFW_KEY_C,                Key::C},
        {GLFW_KEY_D,                Key::D},
        {GLFW_KEY_E,                Key::E},
        {GLFW_KEY_F,                Key::F},
        {GLFW_KEY_G,                Key::G},
        {GLFW_KEY_H,                Key::H},
        {GLFW_KEY_I,                Key::I},
        {GLFW_KEY_J,                Key::J},
        {GLFW_KEY_K,                Key::K},
        {GLFW_KEY_L,                Key::L},
        {GLFW_KEY_M,                Key::M},
        {GLFW_KEY_N,                Key::N},
        {GLFW_KEY_O,                Key::O},
        {GLFW_KEY_P,                Key::P},
        {GLFW_KEY_Q,                Key::Q},
        {GLFW_KEY_R,                Key::R},
        {GLFW_KEY_S,                Key::S},
        {GLFW_KEY_T,                Key::T},
        {GLFW_KEY_U,                Key::U},
        {GLFW_KEY_V,                Key::V},
        {GLFW_KEY_W,                Key::W},
        {GLFW_KEY_X,                Key::X},
        {GLFW_KEY_Y,                Key::Y},
        {GLFW_KEY_Z,                Key::Z},
        {GLFW_KEY_LEFT_BRACKET,     Key::LBracket},
        {GLFW_KEY_BACKSLASH,        Key::Backslash},
        {GLFW_KEY_RIGHT_BRACKET,    Key::RBracket},
        {GLFW_KEY_GRAVE_ACCENT,     Key::BackTick},
        {GLFW_KEY_ESCAPE,           Key::Escape},
        {GLFW_KEY_ENTER,            Key::Enter},
        {GLFW_KEY_TAB,              Key::Tab},
        {GLFW_KEY_BACKSPACE,        Key::Backspace},
        {GLFW_KEY_INSERT,           Key::Insert},
        {GLFW_KEY_DELETE,           Key::Delete},
        {GLFW_KEY_RIGHT,            Key::Right},
        {GLFW_KEY_LEFT,             Key::Left},
        {GLFW_KEY_DOWN,             Key::Down},
        {GLFW_KEY_UP,               Key::Up},
        {GLFW_KEY_PAGE_UP,          Key::PageUp},
        {GLFW_KEY_PAGE_DOWN,        Key::PageDown},
        {GLFW_KEY_HOME,             Key::Home},
        {GLFW_KEY_END,              Key::End},
        {GLFW_KEY_CAPS_LOCK,        Key::Caps},
        {GLFW_KEY_SCROLL_LOCK,      Key::ScrollLock},
        {GLFW_KEY_NUM_LOCK,         Key::NumLock},
        {GLFW_KEY_PRINT_SCREEN,     Key::PrintScreen},
        {GLFW_KEY_PAUSE,            Key::Pause},
        {GLFW_KEY_F1,               Key::F1},
        {GLFW_KEY_F2,               Key::F2},
        {GLFW_KEY_F3,               Key::F3},
        {GLFW_KEY_F4,               Key::F4},
        {GLFW_KEY_F5,               Key::F5},
        {GLFW_KEY_F6,               Key::F6},
        {GLFW_KEY_F7,               Key::F7},
        {GLFW_KEY_F8,               Key::F8},
        {GLFW_KEY_F9,               Key::F9},
        {GLFW_KEY_F10,              Key::F10},
        {GLFW_KEY_F11,              Key::F11},
        {GLFW_KEY_F12,              Key::F12},
        {GLFW_KEY_F13,              Key::F13},
        {GLFW_KEY_F14,              Key::F14},
        {GLFW_KEY_F15,              Key::F15},
        {GLFW_KEY_F16,              Key::F16},
        {GLFW_KEY_F17,              Key::F17},
        {GLFW_KEY_F18,              Key::F18},
        {GLFW_KEY_F19,              Key::F19},
        {GLFW_KEY_F20,              Key::F20},
        {GLFW_KEY_F21,              Key::F21},
        {GLFW_KEY_F22,              Key::F22},
        {GLFW_KEY_F23,              Key::F23},
        {GLFW_KEY_F24,              Key::F24},
        {GLFW_KEY_F25,              Key::F25},
        {GLFW_KEY_KP_0,             Key::KP0},
        {GLFW_KEY_KP_1,             Key::KP1},
        {GLFW_KEY_KP_2,             Key::KP2},
        {GLFW_KEY_KP_3,             Key::KP3},
        {GLFW_KEY_KP_4,             Key::KP4},
        {GLFW_KEY_KP_5,             Key::KP5},
        {GLFW_KEY_KP_6,             Key::KP6},
        {GLFW_KEY_KP_7,             Key::KP7},
        {GLFW_KEY_KP_8,             Key::KP8},
        {GLFW_KEY_KP_9,             Key::KP9},
        {GLFW_KEY_KP_DECIMAL,       Key::KPPeriod},
        {GLFW_KEY_KP_DIVIDE,        Key::KPDivide},
        {GLFW_KEY_KP_MULTIPLY,      Key::KPMultiply},
        {GLFW_KEY_KP_SUBTRACT,      Key::KPSubstract},
        {GLFW_KEY_KP_ADD,           Key::KPAdd},
        {GLFW_KEY_KP_ENTER,         Key::KPEnter},
        {GLFW_KEY_KP_EQUAL,         Key::KPEqual},
        {GLFW_KEY_LEFT_SHIFT,       Key::LShift},
        {GLFW_KEY_LEFT_CONTROL,     Key::LControl},
        {GLFW_KEY_LEFT_ALT,         Key::LAlt},
        {GLFW_KEY_LEFT_SUPER,       Key::LSystem},
        {GLFW_KEY_RIGHT_SHIFT,      Key::RShift},
        {GLFW_KEY_RIGHT_CONTROL,    Key::RControl},
        {GLFW_KEY_RIGHT_ALT,        Key::RAlt},
        {GLFW_KEY_RIGHT_SUPER,      Key::RSystem},
        {GLFW_KEY_MENU,             Key::Menu}        
    });

    bool GLFWInput::isKeyPressedImpl(Key key)
    {
        auto glfw_window = (GLFWwindow*)m_activeWindow->getPlatformWindow();
        return (glfwGetKey(glfw_window, m_cometKeyToGLFWKeyMap[key]) == GLFW_PRESS);
    }

    bool GLFWInput::isMouseButtonPressedImpl(MouseButton button)
    {
        auto glfw_window = (GLFWwindow*)m_activeWindow->getPlatformWindow();
        return (glfwGetMouseButton(glfw_window, m_cometButtonToGLFWButtonMap[button]) == GLFW_PRESS);
    }

    std::pair<int, int> GLFWInput::getMousePositionImpl()
    {
        // get the local mouse position (relative to a window)
        return std::make_pair(0, 0);
    }
    
} // namespace comet