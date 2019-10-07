#ifndef VIEWER_H
#define VIEWER_H

class GLFWwindow;

class Viewer
{
public:
    Viewer(void);
    ~Viewer(void);

    void Run(void);
    /* void Setup(void); */

private:
    void registerCallbacks(void);

private:
    GLFWwindow* m_window;
    unsigned int VBO, VAO, EBO;
};

#endif
