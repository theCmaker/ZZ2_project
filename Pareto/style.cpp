#include "style.h"

Style::Style() {}

Style::~Style() {}

bool Style::has_grid() const
{
    return has_grid_;
}

void Style::setHas_grid(bool has_grid)
{
    has_grid_ = has_grid;
}
bool Style::has_axes() const
{
    return has_axes_;
}

void Style::setHas_axes(bool has_axes)
{
    has_axes_ = has_axes;
}
bool Style::has_axes_labels() const
{
    return has_axes_labels_;
}

void Style::setHas_axes_labels(bool has_axes_labels)
{
    has_axes_labels_ = has_axes_labels;
}
float Style::x_step() const
{
    return x_step_;
}

void Style::setX_step(float x_step)
{
    x_step_ = x_step;
}
float Style::y_step() const
{
    return y_step_;
}

void Style::setY_step(float y_step)
{
    y_step_ = y_step;
}
float Style::y_max() const
{
    return y_max_;
}

void Style::setY_max(float y_max)
{
    y_max_ = y_max;
}
float Style::y_min() const
{
    return y_min_;
}

void Style::setY_min(float y_min)
{
    y_min_ = y_min;
}
float Style::x_max() const
{
    return x_max_;
}

void Style::setX_max(float x_max)
{
    x_max_ = x_max;
}
float Style::x_min() const
{
    return x_min_;
}

void Style::setX_min(float x_min)
{
    x_min_ = x_min;
}
std::vector<double> Style::x_ticks() const
{
    return x_ticks_;
}

void Style::setX_ticks(const std::vector<double> x_ticks)
{
    x_ticks_ = x_ticks;
}
std::vector<double> Style::y_ticks() const
{
    return y_ticks_;
}

void Style::setY_ticks(const std::vector<double> y_ticks)
{
    y_ticks_ = y_ticks;
}
std::vector<std::string> Style::x_ticks_labels() const
{
    return x_ticks_labels_;
}

void Style::setX_ticks_labels(const std::vector<std::string> x_ticks_labels)
{
    x_ticks_labels_ = x_ticks_labels;
}
std::vector<std::string> Style::y_ticks_labels() const
{
    return y_ticks_labels_;
}

void Style::setY_ticks_labels(const std::vector<std::string> y_ticks_labels)
{
    y_ticks_labels_ = y_ticks_labels;
}

float Style::width() const
{
    return width_;
}

void Style::setWidth(float width)
{
    width_ = width;
}

float Style::height() const
{
    return height_;
}

void Style::setHeight(float height)
{
    height_ = height;
}

double Style::aspectRatio() const
{
    return aspect_ratio;
}

void Style::setAspectRatio(double value)
{
    aspect_ratio = value;
}
















