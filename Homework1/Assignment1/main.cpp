#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    float rad = rotation_angle * M_PI / 180.0f; 

    float c = std::cos(rad);
    float s = std::sin(rad);

    model(0, 0) = c;
    model(0, 1) = -s;
    model(1, 0) = s;
    model(1, 1) = c;
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    float fov_rad = eye_fov * M_PI / 180.0f;
    float n = -zNear; 
    float f = -zFar;
    
    float t = std::abs(n) * std::tan(fov_rad / 2.0f);
    float b = -t;
    
    float r = t * aspect_ratio;
    float l = -r;

    Eigen::Matrix4f M_persp2ortho = Eigen::Matrix4f::Identity();
    M_persp2ortho(0, 0) = n;
    M_persp2ortho(1, 1) = n;
    M_persp2ortho(2, 2) = n + f;
    M_persp2ortho(2, 3) = -n * f;
    M_persp2ortho(3, 2) = 1;
    M_persp2ortho(3, 3) = 0;

    Eigen::Matrix4f M_ortho = Eigen::Matrix4f::Identity();
    
    M_ortho(0, 0) = 2.0f / (r - l);
    M_ortho(1, 1) = 2.0f / (t - b);
    M_ortho(2, 2) = 2.0f / (n - f); 
    

    Eigen::Matrix4f M_translate = Eigen::Matrix4f::Identity();
    M_translate(0, 3) = -(r + l) / 2.0f;
    M_translate(1, 3) = -(t + b) / 2.0f;
    M_translate(2, 3) = -(n + f) / 2.0f;

    projection = M_ortho * M_translate * M_persp2ortho;

    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
