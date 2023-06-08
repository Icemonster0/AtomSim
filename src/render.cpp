#include "includes.h"

using namespace std;


color frame[res_x][res_y];


void clear_frame() {
    for(int x = 0; x < res_x; x++) {
        for(int y = 0; y < res_y; y++) {
            frame[x][y] = background_color;
        }
    }
}


void print_pnm(int frame_i) {
    ofstream file;

    file.open(output_path + to_string(frame_i) + ".pnm");

    file << "P3" << endl;
    file << res_x << " " << res_y << endl;
    file << "255" << endl;

    for(int y = res_y-1; y >= 0; y--) {
        for(int x = 0; x < res_x; x++) {
            // file << (int)(clamp(frame[x][y].r * 255, 0.0f, 255.0f)) << " ";
            // file << (int)(clamp(frame[x][y].g * 255, 0.0f, 255.0f)) << " ";
            // file << (int)(clamp(frame[x][y].b * 255, 0.0f, 255.0f)) << " ";
            file << (int)(frame[x][y].r * 255) << " ";
            file << (int)(frame[x][y].g * 255) << " ";
            file << (int)(frame[x][y].b * 255) << " ";
        }
        file << endl;
    }

    file.close();
}


void convert_to_png(int frame_i) {
    // comment the following lines if you don't have imagemagick  --v

    stringstream command_1;
    command_1 << "convert " << output_path << frame_i << ".pnm" << " " << output_path << frame_i << ".png";
    system(command_1.str().c_str());

    stringstream command_2;
    command_2 << "rm " << output_path << frame_i << ".pnm";
    system(command_2.str().c_str());

    // --^
}


void draw_spring_tension() {
    for(auto &spring : mesh.spring_list) {
        int frame_x = (spring.atom_a->pos.x + spring.atom_b->pos.x) * 0.5 * scale - offset_x;
        int frame_y = (spring.atom_a->pos.z + spring.atom_b->pos.z) * 0.5 * scale - offset_y;

        if(frame_x < res_x && frame_x >= 0 && frame_y < res_y && frame_y >= 0) {
            // float value = spring.calculate_length() / (2 * spring.resting_length);
            float value = color_scale * (spring.calculate_length() / spring.resting_length - 1) + 0.5;
            value = clamp(value, 0.0f, 1.0f);
            // cout << value << endl;
            // value = 1.0f;

            // frame[frame_x][frame_y].r = 0.4 * value * value;
            // frame[frame_x][frame_y].g = 0.3 * (-powf(2 * value - 1, 2) + 1);
            // frame[frame_x][frame_y].b = -powf(value - 1, 2) + 1;
            frame[frame_x][frame_y].r = value;
            frame[frame_x][frame_y].g = 0.4f;
            frame[frame_x][frame_y].b = -value + 1;
        }
    }
}


void draw_atoms() {
    for(auto &atom : mesh.atom_list) {
        int frame_x = atom.pos.x * scale - offset_x;
        int frame_y = atom.pos.z * scale - offset_y;

        if(frame_x < res_x && frame_x >= 0 && frame_y < res_y && frame_y >= 0)
            frame[frame_x][frame_y] = draw_color;
    }
}


void draw_floor() {
    for(int x = 0; x < res_x; x++) {
        for(int y = 0; y < -offset_y; y++) {
            frame[x][y] = floor_color;
        }
    }
}


void render(int frame_i) {
    clear_frame();

    if(show_tension)
        draw_spring_tension();
    else
        draw_atoms();

    if(floor_enable)
        draw_floor();

    print_pnm(frame_i);
    convert_to_png(frame_i);
}
