/* 
 * File:   Basic.hpp
 * Author: tku
 *
 * Created on 3 listopad 2013, 04:32
 */

#ifndef BASIC_HPP
#define	BASIC_HPP

#include "NGE/Geometry/Mesh.hpp"

namespace NGE
{
    namespace Geometry
    {
        namespace Basic
        {
            class Basic : public Geometry::Mesh
            {
              public:
                /**
                 * Metoda inicjująca obiekt.
                 * Metoda inicjuje obiekt tzn. wywołuje metody InitializeGeometry(), InitializeVBO() i InitializeVBA().
                 * @return true - jeżeli wszystko się powiodło, w przeciwnym razie false.
                 */
                virtual bool Initialize() = 0;
                
                /**
                 * Metoda aktualizująca obiekt.
                 * @param deltaTime
                 */
                virtual void Update(float deltaTime) = 0;
                
                /**
                 * Metoda renderująca obiekt
                 */
                virtual void Render() = 0;
                
              protected:
                
                /**
                 * Metoda inicjująca geometrię.
                 * Uzupełnia wektory wierzchołków, tekstur, wektorów normalnych, indeksów i kolorów.
                 * @return true - jeżeli wszystko się powiodło, w przeciwnym razie false.
                 */
                virtual bool InitializeGeometry() = 0;
                
                /**
                 * Metoda inicjująca Vertex Buffer Objects.
                 * Metoda generująca bufory i wypełniająca je danymi.
                 * @return true - jeżeli wszystko się powiodło, w przeciwnym razie false.
                 */
                virtual bool InitializeVBO() = 0;
                
                /**
                 * Metoda inicjująca Vertex Buffer Arrays.
                 * Metoda generuje VBA i przypisuje do nich bufory obiektów wygenerowane przy pomocy
                 * metody InitializeVBO().
                 * @return true - jeżeli wszystko się powiodło, w przeciwnym razie false.
                 */
                virtual bool InitializeVBA() = 0;
            };
        }
    }
}

#endif	/* BASIC_HPP */

