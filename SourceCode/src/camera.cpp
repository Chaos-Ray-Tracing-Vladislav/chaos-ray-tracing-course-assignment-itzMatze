#include "camera.hpp"

#include "vec.hpp"
#include "random_generator.hpp"

Camera::Camera(const cm::Vec2u& resolution, const cm::Vec3& origin, const cm::Vec3& view_dir, const cm::Vec3& up, float sensor_width, float focal_length)
  : origin(origin), backward(cm::normalize(-view_dir))
{
  // camera coordinate system
  right = cm::normalize(cm::cross(up, backward));
  this->up = cm::cross(backward, right);
  // match sensor aspect ratio to resolution aspect ratio
  float sensor_height = (float(resolution.y) / float(resolution.x)) * sensor_width;
  // pixel size is always quadratic: (sensor_height / float(resolution.y) = sensor_width / float(resolution.x))
  pixel_size = sensor_width * (1.0 / float(resolution.x));
  upper_left_corner = origin + (sensor_height / 2.0) * up - (sensor_width / 2.0) * right - focal_length * backward;
}

Ray Camera::get_ray(const cm::Vec2u& pixel, bool use_jittering)
{
  // offset to either get a random position inside of the pixel square or the center of the pixel
  cm::Vec2 offset = use_jittering ? cm::Vec2(rng::random_float(), rng::random_float()) : cm::Vec2(0.5);
  const cm::Vec3 pixel_pos = upper_left_corner + (float(pixel.x) + offset.x) * pixel_size * right - (float(pixel.y) + offset.y) * pixel_size * up;
  return Ray(origin, pixel_pos - origin);
}

