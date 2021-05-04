/*
 * Disney  DSS-NCP Take Home Exercise v5
 *
 * @author Peyman Jahangirlou
 * @brief Json Parser class:
 *		-	load json from web then read movies from home url (default mode)
 *		-	load json from web then read movies from erf url (default mode)
 *			-	then populates MovieContainer map.
 */

#include "JsonParser.h"


/* @brief default construct, sets default json url to M_urlLink */
JsonParser::JsonParser()
{
	M_urlLink = M_homeUrl;
	M_mode = Mode::default;
}

/** @brief read json file from url by refId **/
bool JsonParser::readByRefId(const string& refId)
{
	M_mode = Mode::refId;
	M_urlLink = M_refUrl + refId + ".json";
	return M_read();
}

 /** @brief  read default json file and appends refId to refIdSet **/
void JsonParser::getRefIds(unordered_set<string>& refIdSet)
{
	if (M_mode == Mode::default &&  M_read()) {
		auto containers = M_root["data"]["StandardCollection"]["containers"];
		auto iter = containers.begin();
		for (; iter != containers.end(); ++iter) {
			auto refId = (*iter)["set"]["refId"];
			if (refId) 
				refIdSet.insert(refId.asString());
		}// end for loop.
	}// end if
}

/** @brief read json file from url by refId 
 * you must call getRefIds to get refId before calling this function 
 */
bool JsonParser::readDefault()
{
	M_mode = Mode::default;
	M_urlLink = M_homeUrl;
	return M_read();
}


/** @brief read each movie from json file then appends to movieSet **/
void JsonParser::getMovieSet(MovieContainer & movieMap)
{
	// get movies in default mode
	if (M_mode == Mode::default) {
		auto movieContainersList = M_root["data"]["StandardCollection"]["containers"];
		for (auto collection : movieContainersList) {
			auto movieSet = collection["set"];
			auto contentClassStr = movieSet["contentClass"].asString();
			std::cout << contentClassStr << std::endl;
			auto movieItems = movieSet["items"];
			M_populateMovieSet(contentClassStr, movieItems, movieMap);
		}// end for loop.

	} else { // get movies in refId mode
		auto subtreeVal = M_root["data"].getMemberNames().front();
		auto subtree = M_root["data"][subtreeVal];
		auto contentClassStr = subtree["contentClass"].asString();
		auto movieItems = subtree["items"];
		M_populateMovieSet(contentClassStr, movieItems, movieMap);
	}// end else statement

}


///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Private Fuction Section /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


/** @brief  curl call back function **/
size_t JsonParser::M_curlWriteToString(void* buffer, size_t size, size_t nmemb, void* userp)
{
	if (!userp)
		return 0;

	auto str_ptr = reinterpret_cast<std::string*>(userp);
	str_ptr->append(reinterpret_cast<char*>(buffer), size * nmemb);
	return size * nmemb;
}

/*
 * @brief  read json file from url then M_root has everything.
 * @return true upon successful read, otherwise false.
 */
bool JsonParser::M_read()
{
	auto curl_ptr = curl_easy_init();
	std::string strBuffer{};
	CURLcode errorCode(CURLE_FAILED_INIT);
	if (curl_ptr) {
		if (   CURLE_OK == (curl_easy_setopt(curl_ptr, CURLOPT_URL, M_urlLink.c_str()))
			&& CURLE_OK == (curl_easy_setopt(curl_ptr, CURLOPT_WRITEFUNCTION, M_curlWriteToString))
			&& CURLE_OK == (curl_easy_setopt(curl_ptr, CURLOPT_WRITEDATA, &strBuffer))) {
				 errorCode = curl_easy_perform(curl_ptr);
		}
	}
	curl_easy_cleanup(curl_ptr);
	Json::Reader reader;
	bool successfull{ true };
	if (errorCode != CURLE_OK || (!reader.parse(strBuffer, M_root))) {
		std::cout << "JSON::ERROR Json::Reader::parse() Failed..." << std::endl;
		successfull = false;
	}
	return successfull;
}


/** @brief helper function -> iterates throug MovieItems and append each to MovieMap **/
void JsonParser::M_populateMovieSet(const string & contentClassStr, 
									const Json::Value & movieItems,
									MovieContainer& movieMap) 
{
	// create movie object
	for (auto item : movieItems) {
		// create movie object.
		auto imageTileSubtree = item["image"]["tile"];
		auto tileFirstMember = imageTileSubtree.getMemberNames().front();
		imageTileSubtree = imageTileSubtree[tileFirstMember];
		auto tileChildMember = imageTileSubtree.getMemberNames().front();
		imageTileSubtree = imageTileSubtree[tileChildMember];
		auto movieImageUrl = imageTileSubtree["default"]["url"].asString();

		auto fullTitleSubtree = item["text"]["title"]["full"];
		auto membersList = fullTitleSubtree.getMemberNames();
		auto firstMemberStr = membersList.front();

		auto movieTitle = fullTitleSubtree[firstMemberStr]["default"]["content"].asString();
		auto movieLanguage = fullTitleSubtree[firstMemberStr]["default"]["language"].asString();
		auto movieSourceEntity = fullTitleSubtree[firstMemberStr]["default"]["sourceEntity"].asString();

		auto movieObj = std::make_shared<Movie>(movieTitle, movieLanguage, movieSourceEntity, movieImageUrl);
		movieMap[contentClassStr].insert(movieObj);
	}// end for loop

}
