/* 
 * File:   SkyDome.hpp
 * Author: tku
 *
 * Created on 17 wrzesień 2013, 16:12
 */

#ifndef SKYDOME_HPP
#define	SKYDOME_HPP

#include "NGE/Geometry/Mesh.hpp"
#include "NGE/Media/Images/Texture.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Rendering/Camera/Camera.hpp"
#include "NGE/Geometry/Nature/SkyColor.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Nature
        {
            enum ColoringMode { COLOR_TEXTURE, COLOR_TABLE, COLOR_MODEL, COLOR_SKYMAP };
            
            class SkyDome : public Mesh
            {
              public:
                SkyDome();

                /**
                 * Metoda inicjująca sferę niebieską
                 * @param radius Promień sfery
                 * @param numSlices 
                 * @param numSides
                 * @param exponential
                 * @param dampening
                 * @return 
                 */
                int Initialize(float radius, int numSlices, int numSides, bool exponential, float dampening = 1);
                
                /**
                 * Metoda ustawiająca parametry dla sfery
                 * TODO: Do modyfikacji, przenieść część rzeczy w jakieś inne miejsce
                 * @param latitude Szerokość geograficzna od 0-360
                 * @param longitude Długość geograficzna od -90 do 90, od południa na północ
                 * @param timeOfDay Czas dnia od 0.0 do 23.99, gdzie 14.25 = 14:15
                 * @param julianDay Dzień wg kalendarza juliańskiego od 1 do 365
                 * @param turbidity Zachmurzenie od 1.0 do 30.0, 2-6 są najbardziej użyteczne dla bezchmurnych dni
                 */
                void SetCoordinates(float latitude, float longitude, float timeOfDay, float julianDay, float turbidity);

                /**
                 * Metoda renderująca sfere
                 */
                void Render();
                
                /**
                 * Metoda renderująca słońce, księżyc i gwiazdy
                 * @param initial TODO: do usunięcia
                 */
                void RenderElements(bool initial);

                /**
                 * Metoda aktualizująca sferę
                 * @param dt Delta czasu od ostatniej klatki
                 */
                void Update(float dt);

                /**
                 * Metoda czyszcząca wszystkie zmienne
                 */
                void Destroy();

                /**
                 * Ustawienie modelu kolorowania nieba
                 * TODO: Prawdopodobnie do usunięcia lub modyfikacji - powinna zostać obsługa modelu analitycznego i teksturowania
                 * @param mode Tryb wg typu wyliczeniowego ColoringMode
                 */
                void SetColoringMode(int mode);
                
                /**
                 * Pobranie modelu kolorowania nieba
                 * @return Aktualnie wybrany model
                 */
                int GetColoringMode();
                
                /**
                 * Pobranie wektora słońca
                 * TODO: Do napisania, aktualnie tylko rama
                 * @return Wektor słońca
                 */
                Math::vec3f GetSunVector();
                
                /**
                 * Pobranie koloru horyzonu w miejscu na które patrzy obserwator
                 * TODO: Do modyfikacji, usunąć parametr wejściowy i użyć kamery podanej prze SetCamera
                 * @param cameraView Wektor patrzenia kamery
                 * @return Kolor horyzontu
                 */
                Math::vec4f GetHorizonColor(const Math::vec3f& cameraView);
                
                /**
                 * Ustawienie shaderu
                 * @param shader Wskaźnik do programu
                 */
                void SetShader(Media::Shaders::GLSLProgram* shader);
                
                /**
                 * Ustawienie tekstury
                 * @param texture Wskaźnik do tekstury
                 */
                void SetTexture(Media::Images::Texture* texture);
                
                /**
                 * Ustawienie kamery
                 * @param camera Wskaźnik do kamery
                 */
                void SetCamera(Rendering::Camera::Camera* camera);
                

              protected:
                GLuint skyMapTexCoordBuffer;
                std::vector<Math::vec2f> skyMapTexCoords;
                
                Math::vec3f* stars;
                Math::vec4f* starColors;
                int numStars;

                ColoringMode coloringMode;
                int numSlices;
                int numSides;
                int numIndices;

                float skyRadius;

                float sunTheta, sunPhi;
                float moonTheta, moonPhi;
                float sliceAng, sideAng;

                float timeOfDay, julianDay, latitude, longitude;
                
                // Zachmuerzenie
                float turbidity;

                SkyColor skyColor;
                Media::Shaders::GLSLProgram* shader;

                Media::Images::Texture* flareTex;
                Media::Images::Texture* moonTex;
                Media::Images::Texture* tex;
                Media::Images::Texture* skyMap;
                
                Rendering::Camera::Camera* camera;

              protected:
                virtual void DeleteBuffers();
                virtual void DeleteArrays();

                void RenderDome();
            };
        }
    }
}

#endif	/* SKYDOME_HPP */

