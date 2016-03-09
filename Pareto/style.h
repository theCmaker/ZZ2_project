#ifndef STYLE_H
#define STYLE_H

#include <vector>
#include <string>

class Style
{
    public:
        Style();
        ~Style();

        bool has_grid() const;
        void setHas_grid(bool);

        bool has_axes() const;
        void setHas_axes(bool);

        bool has_axes_labels() const;
        void setHas_axes_labels(bool);

        float x_step() const;
        void setX_step(float x_step);

        float y_step() const;
        void setY_step(float y_step);

        float y_max() const;
        void setY_max(float y_max);

        float y_min() const;
        void setY_min(float y_min);

        float x_max() const;
        void setX_max(float x_max);

        float x_min() const;
        void setX_min(float x_min);

        std::vector<double> x_ticks() const;
        void setX_ticks(const std::vector<double> x_ticks);

        std::vector<double> y_ticks() const;
        void setY_ticks(const std::vector<double> y_ticks);

        float width() const;
        void setWidth(float width);

        float height() const;
        void setHeight(float height);

        double aspectRatio() const;
        void setAspectRatio(double value);

        int selected_front() const;
        void setSelected_front(int selected_front);

private:
        bool has_grid_;
        bool has_axes_;
        bool has_axes_labels_;

        float x_step_;
        float y_step_;
        float width_;
        float height_;
        double aspect_ratio_;
        std::vector<double> x_ticks_;
        std::vector<double> y_ticks_;

        int selected_front_;
        float x_min_;
        float x_max_;
        float y_min_;
        float y_max_;
};

#endif // STYLE_H
