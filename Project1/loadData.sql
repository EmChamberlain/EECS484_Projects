--USERS
INSERT INTO USERS (USER_ID, FIRST_NAME, LAST_NAME, YEAR_OF_BIRTH, MONTH_OF_BIRTH, DAY_OF_BIRTH, GENDER)
SELECT DISTINCT USER_ID, FIRST_NAME, LAST_NAME, YEAR_OF_BIRTH, MONTH_OF_BIRTH, DAY_OF_BIRTH, GENDER FROM jsoren.PUBLIC_USER_INFORMATION
WHERE USER_ID IN (SELECT DISTINCT USER_ID FROM jsoren.PUBLIC_USER_INFORMATION);

--CITIES


--USER_CURRENT_CITIES 
INSERT INTO CITIES (CITY_NAME, STATE_NAME, COUNTRY_NAME)
SELECT DISTINCT CURRENT_CITY, CURRENT_STATE, CURRENT_COUNTRY FROM jsoren.PUBLIC_USER_INFORMATION;

INSERT INTO USER_CURRENT_CITIES (USER_ID, CURRENT_CITY_ID)
SELECT DISTINCT jsoren.PUBLIC_USER_INFORMATION.USER_ID, CITIES.CITY_ID FROM jsoren.PUBLIC_USER_INFORMATION
INNER JOIN CITIES ON jsoren.PUBLIC_USER_INFORMATION.CURRENT_CITY=CITIES.CITY_NAME
AND jsoren.PUBLIC_USER_INFORMATION.CURRENT_STATE=CITIES.STATE_NAME
AND jsoren.PUBLIC_USER_INFORMATION.CURRENT_COUNTRY=CITIES.COUNTRY_NAME;


--USER_HOMETOWN_CITIES
INSERT INTO CITIES (CITY_NAME, STATE_NAME, COUNTRY_NAME)
SELECT DISTINCT HOMETOWN_CITY, HOMETOWN_STATE, HOMETOWN_COUNTRY FROM jsoren.PUBLIC_USER_INFORMATION
WHERE NOT EXISTS (SELECT * FROM CITIES WHERE CITIES.CITY_NAME=jsoren.PUBLIC_USER_INFORMATION.HOMETOWN_CITY
AND CITIES.STATE_NAME=jsoren.PUBLIC_USER_INFORMATION.HOMETOWN_STATE
AND CITIES.COUNTRY_NAME=jsoren.PUBLIC_USER_INFORMATION.HOMETOWN_COUNTRY
);

INSERT INTO USER_HOMETOWN_CITIES (USER_ID, HOMETOWN_CITY_ID)
SELECT DISTINCT jsoren.PUBLIC_USER_INFORMATION.USER_ID, CITIES.CITY_ID FROM jsoren.PUBLIC_USER_INFORMATION
INNER JOIN CITIES ON jsoren.PUBLIC_USER_INFORMATION.HOMETOWN_CITY=CITIES.CITY_NAME
AND jsoren.PUBLIC_USER_INFORMATION.HOMETOWN_STATE=CITIES.STATE_NAME
AND jsoren.PUBLIC_USER_INFORMATION.HOMETOWN_COUNTRY=CITIES.COUNTRY_NAME;

--PROGRAMS


INSERT INTO PROGRAMS (INSTITUTION, CONCENTRATION, DEGREE)
SELECT DISTINCT INSTITUTION_NAME, PROGRAM_CONCENTRATION, PROGRAM_DEGREE FROM jsoren.PUBLIC_USER_INFORMATION
WHERE INSTITUTION_NAME IS NOT NULL;

--EDUCATION
INSERT INTO EDUCATION (USER_ID, PROGRAM_ID, PROGRAM_YEAR)
SELECT DISTINCT jsoren.PUBLIC_USER_INFORMATION.USER_ID, PROGRAMS.PROGRAM_ID, jsoren.PUBLIC_USER_INFORMATION.PROGRAM_YEAR
FROM jsoren.PUBLIC_USER_INFORMATION
INNER JOIN PROGRAMS ON jsoren.PUBLIC_USER_INFORMATION.INSTITUTION_NAME=PROGRAMS.INSTITUTION
AND jsoren.PUBLIC_USER_INFORMATION.PROGRAM_CONCENTRATION=PROGRAMS.CONCENTRATION
AND jsoren.PUBLIC_USER_INFORMATION.PROGRAM_DEGREE=PROGRAMS.DEGREE;


--FRIENDS
INSERT INTO FRIENDS (USER1_ID, USER2_ID)
SELECT DISTINCT USER1_ID, USER2_ID FROM
(SELECT DISTINCT USER1_ID, USER2_ID FROM jsoren.PUBLIC_ARE_FRIENDS
UNION
SELECT DISTINCT USER2_ID, USER2_ID FROM jsoren.PUBLIC_ARE_FRIENDS)
WHERE USER1_ID < USER2_ID;

--PHOTOS/ALBUMS
SET AUTOCOMMIT OFF;

--PHOTOS
INSERT INTO PHOTOS (PHOTO_ID, ALBUM_ID, PHOTO_CAPTION, PHOTO_CREATED_TIME, PHOTO_MODIFIED_TIME, PHOTO_LINK)
SELECT DISTINCT PHOTO_ID, ALBUM_ID, PHOTO_CAPTION, PHOTO_CREATED_TIME, PHOTO_MODIFIED_TIME, PHOTO_LINK
FROM jsoren.PUBLIC_PHOTO_INFORMATION;

--ALBUMS
INSERT INTO ALBUMS (ALBUM_ID, ALBUM_OWNER_ID, ALBUM_NAME, ALBUM_CREATED_TIME, ALBUM_MODIFIED_TIME, ALBUM_LINK, ALBUM_VISIBILITY, COVER_PHOTO_ID)
SELECT DISTINCT ALBUM_ID, OWNER_ID, ALBUM_NAME, ALBUM_CREATED_TIME, ALBUM_MODIFIED_TIME, ALBUM_LINK, ALBUM_VISIBILITY, COVER_PHOTO_ID
FROM jsoren.PUBLIC_PHOTO_INFORMATION;

COMMIT;
SET AUTOCOMMIT ON;

--TAGS
INSERT INTO TAGS (TAG_PHOTO_ID, TAG_SUBJECT_ID, TAG_CREATED_TIME, TAG_X, TAG_Y)
SELECT DISTINCT PHOTO_ID, TAG_SUBJECT_ID, TAG_CREATED_TIME, TAG_X_COORDINATE, TAG_Y_COORDINATE
FROM jsoren.PUBLIC_TAG_INFORMATION;


--USER_EVENTS
INSERT INTO USER_EVENTS (EVENT_ID, EVENT_CREATOR_ID, EVENT_NAME, EVENT_TAGLINE, EVENT_DESCRIPTION,
EVENT_HOST, EVENT_TYPE, EVENT_SUBTYPE, EVENT_ADDRESS, EVENT_CITY_ID, EVENT_START_TIME, EVENT_END_TIME)
SELECT DISTINCT jsoren.PUBLIC_EVENT_INFORMATION.EVENT_ID, jsoren.PUBLIC_EVENT_INFORMATION.EVENT_CREATOR_ID,
jsoren.PUBLIC_EVENT_INFORMATION.EVENT_NAME, jsoren.PUBLIC_EVENT_INFORMATION.EVENT_TAGLINE,
jsoren.PUBLIC_EVENT_INFORMATION.EVENT_DESCRIPTION, jsoren.PUBLIC_EVENT_INFORMATION.EVENT_HOST,
jsoren.PUBLIC_EVENT_INFORMATION.EVENT_TYPE, jsoren.PUBLIC_EVENT_INFORMATION.EVENT_SUBTYPE,
jsoren.PUBLIC_EVENT_INFORMATION.EVENT_ADDRESS, CITIES.CITY_ID, jsoren.PUBLIC_EVENT_INFORMATION.EVENT_START_TIME,
jsoren.PUBLIC_EVENT_INFORMATION.EVENT_END_TIME
FROM jsoren.PUBLIC_EVENT_INFORMATION
INNER JOIN CITIES ON jsoren.PUBLIC_EVENT_INFORMATION.EVENT_CITY=CITIES.CITY_NAME
AND jsoren.PUBLIC_EVENT_INFORMATION.EVENT_STATE=CITIES.STATE_NAME
AND jsoren.PUBLIC_EVENT_INFORMATION.EVENT_COUNTRY=CITIES.COUNTRY_NAME;
