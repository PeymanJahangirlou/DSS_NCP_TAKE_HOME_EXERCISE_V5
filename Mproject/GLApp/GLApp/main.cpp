/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Main function.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <unistd.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include  "sdb_image.h"
#include "JsonParser.h"
#include "Movie.h"
#include "Utility.h"

using namespace std;

// Global variables
const int WIDTH  = 1024;
const int HEIGHT = 800;

unsigned int texture[1];

typedef unordered_set<std::shared_ptr<Movie>>::iterator MovieSetIterator;
JsonParser::MovieContainer trendingMovieMap{};
JsonParser::MovieContainer personalizedCuratedMap{};
JsonParser::MovieContainer becauseYouSetMap{};
MovieSetIterator trendingMovieIter;
MovieSetIterator personalizeCuratedMoviesIter;
MovieSetIterator  becauseYouSetMoviesIter;

/** @brief sets up texture **/
void setUpTexture(std::shared_ptr<MovieImage> movieImage_ptr)
{
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
			   movieImage_ptr->width(),
			   movieImage_ptr->height(), 0,
	           GL_RGB, GL_UNSIGNED_BYTE,
		       movieImage_ptr->image());
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/** @brief download image from web then load texture **/
void loadTexture(const std::string & imgUrl)
{
	if (!Utility::downloadMovieImage(imgUrl)) {
		cout << "LOASTEXTURE::Utility::downloadMovieImage() Failed\n" << endl;
		return;
	}
   const string imgPath{ "movieImage.jpg" };
   auto movieImage_ptr = Utility::loadImageFromDirectory(imgPath);
   if (!movieImage_ptr) {
		cout << "LOASTEXTURE::Utility::loadImageFromDirectory() Failed\n" << endl;
		return;
   }
   setUpTexture(movieImage_ptr);
}

/** @brief creates disney logo on top center of the screen **/
void createDisneyPlusLogo()
{
	const string imgPath{ "disneyPlusLogo.jpg" };
	auto movieImage_ptr = Utility::loadImageFromDirectory(imgPath);
	if (!movieImage_ptr) {
		cout << "LOASTEXTURE::Utility::loadImageFromDirectory() Failed\n" << endl;
		return;
	}
	setUpTexture(movieImage_ptr);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,0.0); glVertex3f(300, HEIGHT - 150, 0.0);  // left bottom
		glTexCoord2f(1.0,0.0); glVertex3f(600, HEIGHT - 150, 0.0);  // right bottom
		glTexCoord2f(1.0,1.0); glVertex3f(600, HEIGHT - 30, 0.0);  // right top
		glTexCoord2f(0.0,1.0); glVertex3f(300, HEIGHT - 30, 0.0);  // left top
	glEnd();
}

/** @brief creates 3 * 3 Grids then add movie image  plus 3 arrow triangles  **/
void createSquares(const float margin,
				   const float squareWidth,
				   const float topY,
				   const float bottomY,
			       unordered_set<std::shared_ptr<Movie>>::iterator iter )
{
		float x_start = 0.0;
		float x_end = squareWidth;
		int countSquere{ 0 };
		while ( x_end < WIDTH  ) {
			if (countSquere != 3) {
				loadTexture(iter->get()->imgUrl()); iter++;
				glBegin(GL_POLYGON);
					glTexCoord2f(0.0,0.0); glVertex3f(x_start+margin, bottomY, 0.0);  // left bottom
					glTexCoord2f(1.0,0.0); glVertex3f(x_end, bottomY, 0.0);  // right bottom
					glTexCoord2f(1.0,1.0); glVertex3f(x_end, topY, 0.0);  // right top
					glTexCoord2f(0.0,1.0); glVertex3f(x_start+margin, topY, 0.0);  // left top
				glEnd();
			} else {
				glBegin(GL_TRIANGLES);
					glVertex3f(x_start+margin + 50, bottomY + 30, 0.0);  // left bottom
					glVertex3f(x_start+margin + 50, topY - 30, 0.0);  // left top
					glVertex3f(x_end - 40, topY - 85, 0.0);  // right top
				glEnd();
			}
			x_start = x_end;
			x_end += squareWidth;
			countSquere++;
		}// end while loop.
}


/** @brief display function for GLUT  **/
void display(void)
{
    glClearColor(0.0f, 0.05f, 0.13f,1.0f);         // disney plus background
	glClear(GL_COLOR_BUFFER_BIT);
	glGenTextures(1,texture);
	const float Margin{ 10.0 };
	const float SquareWidth{ 250.0 };
	float stripBottomY{ 50 };
	float stripTopY{ 250 };
	bool first(true);

	// create disney + logo
	createDisneyPlusLogo();

	createSquares(Margin, SquareWidth, stripTopY - Margin, stripBottomY + Margin, trendingMovieIter);
	stripBottomY = stripTopY + Margin;
	stripTopY += 200;

	// create 2nd row
	createSquares(Margin, SquareWidth, stripTopY - Margin, stripBottomY + Margin, personalizeCuratedMoviesIter);
	stripBottomY = stripTopY + Margin;
	stripTopY += 200;

	createSquares(Margin, SquareWidth, stripTopY - Margin, stripBottomY + Margin, becauseYouSetMoviesIter );
	stripBottomY = stripTopY + Margin;
	stripTopY += 200;

	glutSwapBuffers(); // double buffered - swap the front and back buffers.
	glDeleteTextures(1, texture);

}


/* @ Keyboard input processing routine. work in progress **/
void keyInput(unsigned char key, int x, int y)
{
   std::cout << "x: " << x << "y: " << y << std::endl;
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

/*
/** @brief Callback routine for non-ASCII key entry
 ** work in progress for moving on the screen arrow keys
 **
void specialKeyInput(int key, int x, int y)
{
	std::cout << "x: " << x << "y: " << y << std::endl;
   if (key == GLUT_KEY_LEFT) 
   {
   }
   if (key == GLUT_KEY_RIGHT) 
   {
   }
}
*/


/** @brief main function **/
int main(int argc, char** argv)
{
	JsonParser json_parser;
	const string trendigRefId{ "25b87551-fd19-421a-be0f-b7f2eea978b3" };
	json_parser.readByRefId(trendigRefId);
	json_parser.getMovieSet(trendingMovieMap);
	trendingMovieIter = trendingMovieMap["TrendingSet"].begin();

	const string personalizedCuratedRefId{ "f506622c-4f75-4f87-bafe-3e08a4433914" };
	json_parser.readByRefId(personalizedCuratedRefId);
	json_parser.getMovieSet(personalizedCuratedMap);
	personalizeCuratedMoviesIter = personalizedCuratedMap["PersonalizedCuratedSet"].begin();

	const string becauseYouSetRefId{ "bd1bfb9a-bbf7-43a0-ac5e-3e3889d7224d" };
	json_parser.readByRefId(becauseYouSetRefId);
	json_parser.getMovieSet(becauseYouSetMap);
	becauseYouSetMoviesIter = becauseYouSetMap["editorial"].begin();

	glutInit(&argc, argv);	// initializes GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		// enbale double buffered mode.
	glutInitWindowSize(WIDTH, HEIGHT)						;		// specifies window size
	glutInitWindowPosition(100, 100);						    	// position the window's initial top-left corner.
	glutCreateWindow("Disney Plus");								// create main window

	glMatrixMode(GL_MODELVIEW);										// setup viewing projection
	glLoadIdentity();												// start with identity matrix
	glOrtho(0.0, WIDTH, 0.0, HEIGHT, -1.0, 1.0);				    // set clipping area of 2D orthographic view
	glutDisplayFunc(display);								    	// registers callback functions
	glutKeyboardFunc(keyInput);								        // registers keyboard event
//	glutSpecialFunc(specialKeyInput);							    // registers function keys.
	glutMainLoop();									         		// infinite main loop

	return EXIT_SUCCESS;
}



