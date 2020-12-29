#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>
#include <iomanip>
#include "shaderprogram.hpp"
#include "vec2.hpp"

/*
Erencan �NANCI 2-A B171210308
*/
using namespace std;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void setup_shader();
void setup_data();
void renk_Degistir();

const unsigned int height = 800;
const unsigned int length = 800;
unsigned int fragmentShader;
unsigned int vertexShader;
unsigned int shaderProgram;
unsigned int VBO;
unsigned int VAO;
float temp = 0.5f;
float vertices[] = {
     //birinci ucgen
     -temp, -temp, 0.0f,        //sol alt kose
      temp, -temp, 0.0f,        //sag alt kose
     -temp,  temp, 0.0f,        //sola dayali ust kose
     //ikinci ucgen
     -temp,  temp, 0.0f,        //sola dayali ust kose
      temp, -temp, 0.0f,        //sag alt
      temp,  temp, 0.0f         //sol ust
};
float xDeger = 4.3f;
float yDeger = 0.0f;
bool isRight = false;
bool isLeft = false;

int main() {
    cout << "Cerceve ciziliyor.." << endl;
    //k�t�phane init
    glfwInit();
    //configure
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Mac-OS X kullan�yorsak bu sat�r� ekliyoruz.

    //window object
    GLFWwindow* window = glfwCreateWindow(height, length, "Proje", NULL, NULL); //bu fonksiyon height,length, title, monitor ve share parametrelerini al�yor. son 2 sini NULL verebilirim.
    cout << "Pencere objesi olusturuldu.." << endl;
    if (window == NULL) {
        cout << "GLFW Penceresi olusturulamadi." << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //pencere boyutu resizable fonksiyonu

    //opengl fonksiyonlar�n� �a��rmadan �nce GLAD'i init etmemiz gerek.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //hangi i�letim sisteminde �al��aca��m�z� bu k�s�mda anlay�p fonksiyonlar� ona g�re derliyor.
        cout << "GLAD baslatilamadi." << endl;
        return -1;
    }

    //render size given to openGL
    glViewport(0, 0, height, length); //ilk 2 parametre pencerenin sol alt noktas� oluyor, sonraki parametreler height ve length de�erlerimiz ile ayn�.
    
    ShaderProgram program;
    program.attachShader("./VertexShader.glsl", GL_VERTEX_SHADER);
    program.attachShader("./FragmentShader.glsl", GL_FRAGMENT_SHADER);
    program.link();
    cout << "Shader'lar olusturulup baglandi.." << endl;

    program.addUniform("uMoveX");
    program.addUniform("uMoveY");
    program.addUniform("ourColor");
    setup_data();
    
    //render loop kurmam�z gerekiyor ekrana s�rekli bi �eyler �izilmesi i�in
    while (!glfwWindowShouldClose(window)) { //true olursa render loop duruyor.
        processInput(window);

        //bu aral��a rendering command'ler gelecek �rne�in background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //RGBA
        glClear(GL_COLOR_BUFFER_BIT); //g�r�nt�y� sil

        program.use();

        //HAREKET KODU �cgenin olustu�u yer orijin (0.0) noktas�
        if (xDeger <= 0 && xDeger >= -4.3f)
        {
            /*isLeft = true;
            isRight = false;*/
        }
        if (xDeger >= 0 && xDeger <= 4.3f)
        {
            isRight = true;
            isLeft = false;
        }

        if (isLeft)
        {
            program.setFloat("uMoveX", xDeger += 0.015f); //saga hareket +x
            if (xDeger >= 4.29f) 
            {
                isRight = true;
                isLeft = false;
            }
        }
        if (isRight) 
        {
            program.setFloat("uMoveX", xDeger -= 0.015f); //sola hareket -x
            if (xDeger <= -4.29f) 
            {
                isLeft = true;
                isRight = false;
            }
        }

        cout << "xDeger : " << xDeger << "   :   " << "isLeft : " << isLeft << "  -----  " << "isRight : " << isRight << endl;

        //�izimde kullan�lacak olan Vertex array object aktif ediliyor
        glBindVertexArray(VAO);
        //�izim komutu g�nderiliyor
        glDrawArrays(GL_TRIANGLES, 0, 6); //�iz
        program.setFloat("uMoveY", 2.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        program.setFloat("uMoveY", yDeger - 2.0f);

        glfwSwapBuffers(window); //yeni resmi monit�re g�ndermek i�in �er�eve tamponlar� de�i�ecek
        glfwPollEvents(); //inputlar� kontrol eden fonksiyon
    }
    glfwTerminate();
    cout << "Pencere kapadildi.." << endl;
    return 0;
}

void processInput(GLFWwindow* window){ //input kontrol�
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true); //esc ile pencereyi kapat
    }
}

//kullan�c� taraf�ndan resizable �zelli�i aktif ediliyor. pencere boyutu de�i�tirilebilir oldu art�k.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void setup_data() {
    //vertex array object olu�turuluyor
    glGenVertexArrays(1, &VAO);
    //vertex buffer object olu�uruluyor
    glGenBuffers(1, &VBO);
    //vertex array object aktif edildi.
    glBindVertexArray(VAO);
    //vertex buffer nesnesi aktif edildi.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //vertex buffer'a nokta bilgileri ytollan�yor
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    //g�nderilen vertex'e ait �zellikler etiketleniyor
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //�zellik  etiket i aktif ediliyor.
    glEnableVertexAttribArray(0);
}

void renk_Degistir() {
    float timeValue = glfwGetTime();
    float redValue = (sin(timeValue) / 2.0f) + 0.5f;
    float greenValue = (sin(timeValue) / 3.0f) + 0.25f;
    float bluValue = (sin(timeValue) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);
}