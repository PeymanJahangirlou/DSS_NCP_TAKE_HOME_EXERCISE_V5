/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Utility class -- contains utility functions
 */

#include "Utility.h"


/** @brief helper function -> saves loaded image from we to solution direction **/
size_t Utility::M_curlSaveImgToFile(void* buffer, size_t size, size_t nmemb, void* userp)
{
	if (userp) {
		ostream & os = *static_cast<ostream*>(userp);
		if (os.write(static_cast<char*>(buffer), size * nmemb))
			return  size * nmemb;
	}

	return 0;
}

/** @brief sets up curl to download image from provided imgUrl **/
CURLcode Utility::downloadMovieImage(const string & imgUrl, ostream & os, long timeout)
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURLcode code(CURLE_FAILED_INIT);
	auto curl_ptr = curl_easy_init();

	if (curl_ptr)
	{
		if (   CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_WRITEFUNCTION, &M_curlSaveImgToFile))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_NOPROGRESS, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_FOLLOWLOCATION, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_FILE, &os))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_TIMEOUT, timeout))
			&& CURLE_OK == (code = curl_easy_setopt(curl_ptr, CURLOPT_URL, imgUrl.c_str())))
		{
			code = curl_easy_perform(curl_ptr);
		}
		curl_easy_cleanup(curl_ptr);
	}
	curl_global_cleanup();
	return code;
}

/** @brief download movie image from provided imgUrl to solution directory **/
bool  Utility::downloadMovieImage(const string& imgUrl)
{
	// first download image to solution directory
	std::ofstream ofs("movieImage.jpg", std::ostream::binary);
	if (CURLE_OK != downloadMovieImage(imgUrl, ofs))
		return false;

	return true;
	/*
	// create MovieImage obj
	const string movieImagePath{ "C:\\dev\\Mproject\\GLApp\\GLApp\\movieImage.jpg" };
    int image_width, image_height, numColCh;
    stbi_set_flip_vertically_on_load(true);

	return   std::make_shared<MovieImage>(image_ptr, image_width, image_height);

	*/
}

/** @brief load image from directory
 * @return MovieImage pointer
 */
std::shared_ptr<MovieImage> Utility::loadImageFromDirectory(const string& imgPath)
{ 
    int width, height, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image_ptr = stbi_load(imgPath.c_str(), &width, &height, &numColCh, 0);
    return std::make_shared<MovieImage>(image_ptr, width, height); 
}
