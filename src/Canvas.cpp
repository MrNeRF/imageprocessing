#include "Canvas.h"
#include "Eigen/Dense"
#include "Image.h"
#include "Rectangle.h"
#include "Shader.h"
#include "TextureObject.h"
#include "cassert"

Canvas::Canvas(const Image& image)
{
    spImageShader = std::make_unique<Shader>("ImageShader");
    spImageShader->InitShaders("../Shaders/image.vs", "../Shaders/image.fs");

    spRectangle     = std::make_unique<Rectangle>(Eigen::Vector2f(-0.5f, -0.5f), 1.0f, 1.0f);
    spTextureObject = std::make_unique<TextureObject>(image);
    spImageShader->UseShader();
    spImageShader->SetValue("texture1", 0);
}

void Canvas::Draw(void)
{
    spImageShader->UseShader();
    spTextureObject->Bind();
    spRectangle->Draw();
}
