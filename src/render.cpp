#include "includes.h"

using namespace std;


color frame[res_X][res_Y];
float inverse_scale = 1 / scale;


void clear_frame() {
    for(int x = 0; x < res_X; x++) {
        for(int y = 0; y < res_Y; y++) {
            frame[x][y] = background_color;
        }
    }
}


void print_pnm(int frame_i) {
    ofstream file;

    file.open(output_path + to_string(frame_i) + ".pnm");
    if(!file.is_open()) {
        cout << "Failed to open file in  render.cpp: print_pnm" << endl;
        exit(1);
    }

    file << "P3" << endl;
    file << res_X << " " << res_Y << endl;
    file << "255" << endl;

    for(int y = res_Y-1; y >= 0; y--) {
        for(int x = 0; x < res_X; x++) {
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
    stringstream command_1;
    command_1 << "convert " << output_path << frame_i << ".pnm" << " " << output_path << frame_i << ".png";
    if(system(command_1.str().c_str())) {
        cout << "Failed to convert. Read the README for a possible solution." << endl;
        exit(1);
    }

    stringstream command_2;
    command_2 << "rm " << output_path << frame_i << ".pnm";
    if(system(command_2.str().c_str())) {
        cout << "Failed to remove pnm." << endl;
    }
}


bool compare_tri_depth(tri tri1, tri tri2) {
    float mid1 = (tri1.a->pos.y + tri1.b->pos.y + tri1.c->pos.y) / 3;
    float mid2 = (tri2.a->pos.y + tri2.b->pos.y + tri2.c->pos.y) / 3;
    return (mid1 < mid2);
}


void sort_tris() {
    sort(mesh.tri_list.begin(), mesh.tri_list.end(), compare_tri_depth);
}


vec3 get_normal(tri tri) {
    vec3 a = tri.b->pos - tri.a->pos;
    vec3 b = tri.c->pos - tri.a->pos;

    vec3 normal = vec3 (a.y * b.z - a.z * b.y,
                        a.z * b.x - a.x * b.z,
                        a.x * b.y - a.y * b.x);

    float l = sqrtf(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);

    return vec3(normal.x / l, normal.y / l, normal.z / l);
}


float get_light_intensity(tri tri) {
    vec3 normal = get_normal(tri);

    float l = sqrtf(light_direction.x*light_direction.x + light_direction.y*light_direction.y + light_direction.z*light_direction.z);
    vec3 light = vec3(light_direction.x / l, light_direction.y / l, light_direction.z / l);

    return clamp(normal.x*light.x + normal.y*light.y + normal.z*light.z, 0.0f, 1.0f);
}


bool get_side(vec3 a, vec3 b, vec3 c) { // returns on which side of the line a--b c is on
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x) > 0;
}


void draw_tris() {
    sort_tris();

    for(auto &tri : mesh.tri_list) {
        float brightness = get_light_intensity(tri);

        // if(brightness > 0) {
            vec3 a = vec3((tri.a->pos.x - offset_X) * inverse_scale * res_X, (tri.a->pos.z - offset_Y) * inverse_scale * res_X, 0);
            vec3 b = vec3((tri.b->pos.x - offset_X) * inverse_scale * res_X, (tri.b->pos.z - offset_Y) * inverse_scale * res_X, 0);
            vec3 c = vec3((tri.c->pos.x - offset_X) * inverse_scale * res_X, (tri.c->pos.z - offset_Y) * inverse_scale * res_X, 0);

            int min_x = min(min(a.x, b.x), c.x);
            int min_y = min(min(a.y, b.y), c.y);
            int max_x = max(max(a.x, b.x), c.x);
            int max_y = max(max(a.y, b.y), c.y);

            vec3 center = vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, 0);

            bool inside_a = get_side(a, b, center);
            bool inside_b = get_side(b, c, center);
            bool inside_c = get_side(c, a, center);

            for(int x = min_x; x <= max_x; x++) {
                for(int y = min_y; y <= max_y; y++) {
                    if(x < res_X && x >= 0 && y < res_Y && y >= 0 &&
                       get_side(a, b, vec3(x, y, 0)) == inside_a &&
                       get_side(b, c, vec3(x, y, 0)) == inside_b &&
                       get_side(c, a, vec3(x, y, 0)) == inside_c) {

                        frame[x][y] = color(brightness, brightness, brightness);
                    }
                }
            }
        // }
    }
}


void draw_spring_tension() {
    #pragma omp parallel for schedule(static)
    for(auto &spring : mesh.spring_list) {
        int frame_X = ((spring.atom_a->pos.x + spring.atom_b->pos.x) * 0.5 - offset_X) * inverse_scale * res_X;
        int frame_Y = ((spring.atom_a->pos.z + spring.atom_b->pos.z) * 0.5 - offset_Y) * inverse_scale * res_X;

        if(frame_X < res_X && frame_X >= 0 && frame_Y < res_Y && frame_Y >= 0) {
            // float value = spring.calculate_length() / (2 * spring.resting_length);
            float value = color_scale * (spring.calculate_length() / spring.resting_length - 1) + 0.5;
            value = clamp(value, 0.0f, 1.0f);
            // cout << value << endl;
            // value = 1.0f;

            // frame[frame_x][frame_y].r = 0.4 * value * value;
            // frame[frame_x][frame_y].g = 0.3 * (-powf(2 * value - 1, 2) + 1);
            // frame[frame_x][frame_y].b = -powf(value - 1, 2) + 1;
            frame[frame_X][frame_Y].r = value;
            frame[frame_X][frame_Y].g = 0.4f;
            frame[frame_X][frame_Y].b = -value + 1;
        }
    }
}


void draw_atoms() {
    #pragma omp parallel for schedule(static)
    for(auto &atom : mesh.atom_list) {
        int frame_x = (atom.pos.x - offset_X) * inverse_scale * res_X;
        int frame_y = (atom.pos.z - offset_Y) * inverse_scale * res_X;

        if(frame_x < res_X && frame_x >= 0 && frame_y < res_Y && frame_y >= 0)
            frame[frame_x][frame_y] = draw_color;
    }
}


void draw_floor() {
    #pragma omp parallel for schedule(static)
    for(int x = 0; x < res_X; x++) {
        for(int y = 0; y <= -offset_Y * inverse_scale * res_X; y++) {
            if(y < res_Y)
                frame[x][y] = floor_color;
        }
    }
}


void render(int frame_i) {
    clear_frame();

    if(show_surface)
        draw_tris();
    if(show_tension)
        draw_spring_tension();
    if(show_atoms)
        draw_atoms();
    if(floor_enable)
        draw_floor();

    print_pnm(frame_i);

    if(output_png)
        convert_to_png(frame_i);
}
