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
        void setHas_grid(bool has_grid);

        bool has_axes() const;
        void setHas_axes(bool has_axes);

        bool has_axes_labels() const;
        void setHas_axes_labels(bool has_axes_labels);

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

        std::vector<std::string> x_ticks_labels() const;
        void setX_ticks_labels(const std::vector<std::string> x_ticks_labels);

        std::vector<std::string> y_ticks_labels() const;
        void setY_ticks_labels(const std::vector<std::string> y_ticks_labels);

private:
        bool has_grid_;
        bool has_axes_;
        bool has_axes_labels_;

        float x_step_;
        float y_step_;
        std::vector<double> x_ticks_;
        std::vector<double> y_ticks_;
        std::vector<std::string> x_ticks_labels_;
        std::vector<std::string> y_ticks_labels_;


        float y_max_;
        float y_min_;
        float x_max_;
        float x_min_;
};

#endif // STYLE_H
