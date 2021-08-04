#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <cyclone.h>
#include <app.h>
#include <math.h>
#include <timing.h>
#include <ogl_header.h>

using namespace std;

//object names and miscellaneous
enum OBJECT_NAME { OBJ_SUN_MARK, OBJ_EARTH, OBJ_EARTH_MARK, OBJ_MOON, OBJ_MOON_MARK};
double PI_2_val = 6.283185;

// view settings
int viewMode = 1; 
int sun_axes = 1; // whether or not to show the 3 axes of the SUN system
int eth_axes = 1; // whether or not to show the 3 axes of the Earth system
int mun_axes = 1;  // whether or not to show the 3 axes of the Moon system
int connexon = 1;  //weather or not to show the connection lines
float USER_HGT = 20.0f;
float user_hgt = USER_HGT;
float user_eye[3];
float focusPnt[3];
float upVector[3];

// rotation mode of EACH object (0: Yaw, 1: Roll, 2: Pitch)
int sun_mode = 1; // roll
int eth_mode = 2; // pitch
int munMode0 = 0; //yaw ... self rotation
int munMode1 = 1; //roll ... revolution around the earth

//object size, track size, and mark size: the sun, the earth, and the moon
float sun_size = 2.00;
float sRefSize = sun_size / 8.0;

float ethTrack = 16.0;
float eth_size = 1.0;
float eRefSize = eth_size / 8.0;

float munTrack = 4.00;
float mun_size = 0.50;
float mRefSize = mun_size / 4.0;

// rotation periods and rotational velocity magnitude values
int theScale = 1;                        // the ratio = speedAdj : SPEEDADJ
float SPEEDADJ = 0.005000;               // speed adjustment used to suit the OpenGL scene: the MINIMUM
float speedAdj = SPEEDADJ * theScale;    //speed adjustment used to suit the size of the OpenGL scene: a multiple of the MINIMUM
float s_self_R = 27.27500;               // self-rotation period of sun
float e_self_R = 1.00;                   //self-rotation period of the earth: 1.0 day = 24 hours
float ethSun_R = 365.25636;            // period of the earth rotating around the sun
float m_self_R = 27.321660;             // self-rotation period of the moon
float munEth_R = 27.321660;             // period of rotation of the moon around the earth: the same as the self-rotation period

double sunSpeed; // self-rotation speed of the sun
double ethSpeed; // self-rotation speed of earth
double e2sSpeed; // self-rotation speed of earth relative to sun
double munSpeed; // self-rotation speed of the moon
double m2eSpeed; // rotation speed of moon relative to earth

// sine and cosine values (used for constructing quaternion)
float s_self_c; // sun cosine for the self-rotation
float s_self_s; // sun sine for the self-rotation
float e_self_c; // earth cosine for self-rotation
float e_self_s; // earth sine for the self-rotation
float e2sCosin; // earth relative to the sun cosine for the rotation
float e2s_sine; // earth relative to sun sine for the rotation
float m_self_c; // moon cosine for the self-rotation
float m_self_s; // moon sine for the self-rotation
float m2eCosin; // moon relative to the earth cosine for the rotation
float m2e_sine; // moon relative to the earth sine for the rotation

// object colors
cyclone::Vector3 sunColor = cyclone::Vector3(1.0, 0.0, 0.0); // the sun: red
cyclone::Vector3 earthClr = cyclone::Vector3(0.0, 0.0, 1.0); // the earth: blue
cyclone::Vector3 moon_clr = cyclone::Vector3(1.0, 1.0, 0.0); // the moon: yellow

// mark colors
cyclone::Vector3 s_mColor = cyclone::Vector3(0.0, 1.0, 0.0); // sun mark: green
cyclone::Vector3 e_mColor = cyclone::Vector3(0.0, 1.0, 0.0); // earth mark: green
cyclone::Vector3 m_mColor = cyclone::Vector3(1.0, 0.0, 0.0); // moon mark: red

// system axes (for the Sun, Earth, and Moon): 3D coordinate of the TIP of EACH axis Line
cyclone::Vector3 sLN_TIPS[3] = { cyclone::Vector3(1.0, 0.0, 0.0) * sun_size * 2.0,   // sun: FIXED
								cyclone::Vector3(0.0, 1.0, 0.0) * sun_size * 2.0,
								cyclone::Vector3(0.0, 0.0, 1.0) * sun_size * 2.0 
								}; 
cyclone::Vector3 eLN_TIPS[3] = { cyclone::Vector3(1.0, 0.0, 0.0) * sun_size * 2.0,
								cyclone::Vector3(0.0, 1.0, 0.0) * sun_size * 2.0,   // earth: initial
								cyclone::Vector3(0.0, 0.0, 1.0) * sun_size * 2.0 
								}; 
cyclone::Vector3 mLN_TIPS[3] = { cyclone::Vector3(1.0, 0.0, 0.0) * sun_size * 2.0, // moon: initial
								cyclone::Vector3(0.0, 1.0, 0.0) * sun_size * 2.0,
								cyclone::Vector3(0.0, 0.0, 1.0) * sun_size * 2.0 
							   };  

cyclone::Vector3 e_LnTips[3]; // earth: dynamically changing
cyclone::Vector3 m_LnTips[3]; // moon: dynamically changing

// position and rotation of the sun
cyclone::Vector3 sun_cord;     // the position
cyclone::Vector3 sun_axis;     // the axis of self-rotation
cyclone::Vector3 SUN_MARK;    // the INITIAL position of the mark
cyclone::Vector3 sun_mark;    // the dynamically changing position of the mark
cyclone::Quaternion sun_quat; // the ACCUMULATED self-rotation of the sun
cyclone::Quaternion s_self_q; // the self-rotation of the sun performed / added in EACH frame

// position and rotation of earth
cyclone::Vector3 earthPOS; // the INITIAL position of the earth
cyclone::Vector3 earthPos; // the dynamically changing position of the earth
cyclone::Vector3 earthAOR; // the axis of self-rotation
cyclone::Vector3 ETH_MARK; // the INITIAL position of the mark
cyclone::Vector3 eth_mark; // the dynamically changing position of the mark
cyclone::Quaternion eSELF_qA; // the ACCUMULATED self-rotation of the earth
cyclone::Quaternion eSELF_qB; // the self-rotation of the earth performed / added in EACH frame

cyclone::Vector3 e2s_axis; // the axis of rotation of the earth around the sun
cyclone::Quaternion e2sQuatA; // the ACCUMULATED rotation of the earth around the sun
cyclone::Quaternion e2sQuatB; // the rotation of the earth around the sun performed / added in EACH frame

// position and rotation of the moon
cyclone::Vector3 moon_POS; // the INITIAL position of the moon
cyclone::Vector3 moon_pos; // the dynamically changing position of the moon
cyclone::Vector3 moonAxis; // the axis of self-rotation
cyclone::Vector3 moonMARK; // the INITIAL position of the mark
cyclone::Vector3 moonMark; // the dynamically changing position of the mark
cyclone::Quaternion mSELF_qA; // the ACCUMULATED self-rotation of the moon
cyclone::Quaternion mSELF_qB; // the self-rotation of the moon performed / added in EACH frame

cyclone::Vector3 M2E_AXIS; // the INITIAL axis of rotation of the moon around the earth
cyclone::Vector3 m2e_axis; // the dynamically changing axis of rotation of the moon around the earth
cyclone::Quaternion m2eQuatA; // the ACCUMULATED rotation of the moon around the earth
cyclone::Quaternion m2eQuatB; // the rotation of the moon around the earth performed / added in EACH frame

// ========================Class QuatApp: BEGIN ===============================================

class QuatApp : public RigidBodyApplication
{
	virtual void reset();
	virtual void updateObjects(cyclone::real duration);

public:
	QuatApp();

	void ApplyDefaultContext();

	// view mode
	void sunView();
	void EarthRadialView();
	void EarthTangentView();
	void EarthTopDownView();
	void MoonRadialView();
	void MoonTangentView();
	void MoonTopDownView();

	//self-rotation modes of the sun
	void Sun_SelfRotation_Yaw();
	void Sun_SelfRotation_Roll();
	void Sun_SelfRotation_Pitch();

	// self-rotation modes of the earth
	void Earth_SelfRotation_Yaw();
	void Earth_SelfRotation_Roll();
	void Earth_SelfRotation_Pitch();

	// self-rotation modes of moon
	void Moon_SelfRotation_Yaw();
	void Moon_SelfRotation_Roll();
	void Moon_SelfRotation_Pitch();

	// the rotation modes of the moon around the earth
	void Moon_AroundEarth_Yaw();
	void Moon_AroundEarth_Roll();
	void Moon_AroundEarth_Pitch();

	// calculation of the position
	void UpdatePosition(cyclone::Quaternion *deltQuat, cyclone::Quaternion *crntQuat,
						cyclone::Vector3 *parntPos, cyclone::Vector3 *this_pos,
						cyclone::Vector3 *that_pos, OBJECT_NAME obj_name);
	void ApplyAngularVelocity(cyclone::Vector3 &axis_rot, 
							  float theCycle, double *rotSpeed, 
							  float *cosValue, float *sinValue,
							  cyclone::Quaternion *deltQuat);
	void ApplyAngularVelocity();

	// rendering and interaction
	void DrawSphere(cyclone::Vector3 &sferePos, cyclone::Vector3 &sfereClr, float sfereSiz, int sfereRes);
	void DrawMark(cyclone::Vector3 &mark_pos, float mark_rad, int mark_res, cyclone::Vector3 &mark_clr);

	void DrawSystemAxes(cyclone::Vector3 *originPt, cyclone::Vector3 *arwHeads);
	void DrawConnectionLines();
	void SetLightMaterial(float x, float y, float z);

	virtual void key(unsigned char key);
	virtual void initGraphics();
	virtual const char* getTitle();
	virtual void display();

};

QuatApp::QuatApp() : RigidBodyApplication()
{
	ApplyDefaultContext();
}

void QuatApp::ApplyAngularVelocity()
{
	ApplyAngularVelocity(sun_axis, s_self_R, &sunSpeed, &s_self_c, &s_self_s, &s_self_q);
	ApplyAngularVelocity(earthAOR, e_self_R, &ethSpeed, &e_self_c, &e_self_s, &eSELF_qB);
	ApplyAngularVelocity(moonAxis, m_self_R, &munSpeed, &m_self_c, &m_self_s, &mSELF_qB);
	ApplyAngularVelocity(m2e_axis, munEth_R, &m2eSpeed, &m2eCosin, &m2e_sine, &m2eQuatB);
	ApplyAngularVelocity(e2s_axis, ethSun_R, &e2sSpeed, &e2sCosin, &e2s_sine, &e2sQuatB);
}




void QuatApp::Sun_SelfRotation_Pitch()
{
	sun_mode = 2;
	sun_quat = cyclone::Quaternion();
	s_self_q = cyclone::Quaternion();
	sun_axis = cyclone::Vector3(1.0, 0.0, 0.0);
	SUN_MARK = cyclone::Vector3(0.0, 0.0, sun_size);
	ApplyAngularVelocity(sun_axis, s_self_R, &sunSpeed, &s_self_c, &s_self_s, &s_self_q);
}

void QuatApp::Sun_SelfRotation_Yaw()
{
	sun_mode = 0;
	sun_quat = cyclone::Quaternion();
	s_self_q = cyclone::Quaternion();
	sun_axis = cyclone::Vector3(0.0, 1.0, 0.0);
	SUN_MARK = cyclone::Vector3(sun_size, 0.0, 0.0);
	ApplyAngularVelocity(sun_axis, s_self_R, &sunSpeed, &s_self_c, &s_self_s, &s_self_q);
}

void QuatApp::Sun_SelfRotation_Roll()
{
	sun_mode = 1;
	sun_quat = cyclone::Quaternion();
	s_self_q = cyclone::Quaternion();
	sun_axis = cyclone::Vector3(0.0, 0.0, 1.0);
	SUN_MARK = cyclone::Vector3(0.0, sun_size, 0.0);
	ApplyAngularVelocity(sun_axis, s_self_R, &sunSpeed, &s_self_c, &s_self_s, &s_self_q);
}

void QuatApp::Earth_SelfRotation_Yaw()
{
	eth_mode = 0;
	eSELF_qA = cyclone::Quaternion();
	eSELF_qB = cyclone::Quaternion();
	earthAOR = cyclone::Vector3(0.0, 1.0, 0.0);
	ETH_MARK = cyclone::Vector3(eth_size, 0.0, 0.0);
	ApplyAngularVelocity(earthAOR, e_self_R, &ethSpeed, &e_self_c, &e_self_s, &eSELF_qB);
}

void QuatApp::Earth_SelfRotation_Roll()
{
	eth_mode = 1;
	eSELF_qA = cyclone::Quaternion();
	eSELF_qB = cyclone::Quaternion();
	earthAOR = cyclone::Vector3(0.0, 0.0, 1.0);
	ETH_MARK = cyclone::Vector3(eth_size, 0.0, 0.0);
	ApplyAngularVelocity(earthAOR, e_self_R, &ethSpeed, &e_self_c, &e_self_s, &eSELF_qB);
}


void QuatApp::Earth_SelfRotation_Pitch()
{
	eth_mode = 2;
	eSELF_qA = cyclone::Quaternion();
	eSELF_qB = cyclone::Quaternion();
	earthAOR = cyclone::Vector3(1.0, 0.0, 0.0);
	ETH_MARK = cyclone::Vector3(0.0, 0.0, eth_size);
	ApplyAngularVelocity(earthAOR, e_self_R, &ethSpeed, &e_self_c, &e_self_s, &eSELF_qB);
}


void QuatApp::Moon_SelfRotation_Yaw()
{
	munMode0 = 0;
	mSELF_qA = cyclone::Quaternion();
	mSELF_qB = cyclone::Quaternion();
	moonAxis = cyclone::Vector3(0.0, 1.0, 0.0);
	moonMARK = cyclone::Vector3(mun_size, 0.0, 0.0);
	ApplyAngularVelocity(moonAxis, m_self_R, &munSpeed, &m_self_c, &m_self_s, &mSELF_qB);
}

void QuatApp::Moon_SelfRotation_Roll()
{
	munMode0 = 1;
	mSELF_qA = cyclone::Quaternion();
	mSELF_qB = cyclone::Quaternion();
	moonAxis = cyclone::Vector3(0.0, 0.0, 1.0);
	moonMARK = cyclone::Vector3(mun_size, 0.0, 0.0);
	ApplyAngularVelocity(moonAxis, m_self_R, &munSpeed, &m_self_c, &m_self_s, &mSELF_qB);
}

void QuatApp::Moon_SelfRotation_Pitch()
{
	munMode0 = 2;
	mSELF_qA = cyclone::Quaternion();
	mSELF_qB = cyclone::Quaternion();
	moonAxis = cyclone::Vector3(1.0, 0.0, 0.0);
	moonMARK = cyclone::Vector3(0.0, 0.0, mun_size);
	ApplyAngularVelocity(moonAxis, m_self_R, &munSpeed, &m_self_c, &m_self_s, &mSELF_qB);
}

void QuatApp::Moon_AroundEarth_Yaw()
{
	munMode1 = 0;
	m2eQuatA = cyclone::Quaternion();
	m2eQuatB = cyclone::Quaternion();
	M2E_AXIS = cyclone::Vector3(0.0, 1.0, 0.0);
	moon_POS = cyclone::Vector3(munTrack, 0.0, 0.0);
	m2e_axis = M2E_AXIS;
	ApplyAngularVelocity(m2e_axis, munEth_R, &m2eSpeed, &m2eCosin, &m2e_sine, &m2eQuatB);
}

void QuatApp::Moon_AroundEarth_Roll()
{
	munMode1 = 1;
	m2eQuatA = cyclone::Quaternion();
	m2eQuatB = cyclone::Quaternion();
	M2E_AXIS = cyclone::Vector3(0.0, 0.0, 1.0);
	moon_POS = cyclone::Vector3(munTrack, 0.0, 0.0);
	m2e_axis = M2E_AXIS;
	ApplyAngularVelocity(m2e_axis, munEth_R, &m2eSpeed, &m2eCosin, &m2e_sine, &m2eQuatB);
}

void QuatApp::Moon_AroundEarth_Pitch()
{
	munMode1 = 2;
	m2eQuatA = cyclone::Quaternion();
	m2eQuatB = cyclone::Quaternion();
	M2E_AXIS = cyclone::Vector3(1.0, 0.0, 0.0);
	moon_POS = cyclone::Vector3(0.0, 0.0, munTrack);
	m2e_axis = M2E_AXIS;
	ApplyAngularVelocity(m2e_axis, munEth_R, &m2eSpeed, &m2eCosin, &m2e_sine, &m2eQuatB);
}

void QuatApp::ApplyAngularVelocity(cyclone::Vector3 &axis_rot, float theCycle, double *rotSpeed, float *cosValue, float *sinValue,
	cyclone::Quaternion *deltQuat)
{
	*rotSpeed = -(PI_2_val * speedAdj * 0.5 / theCycle);
	*cosValue = float(cos(*rotSpeed));
	*sinValue = float(sin(*rotSpeed));
	*deltQuat = cyclone::Quaternion((*cosValue), (*sinValue) * axis_rot[0], (*sinValue) * axis_rot[1], (*sinValue) * axis_rot[2]);
}

void QuatApp::SetLightMaterial(float x, float y, float z)
{
	GLfloat ambient[] = { 0.20, 0.20, 0.20, 1.00 };
	GLfloat diffuse[] = { 0.75,0.50,0.15,1.00 };
	GLfloat speclar[] = { 0.80,0.80,0.80,1.00 };
	GLfloat position[] = { x, y, z, 1.00 };

	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.75,0.5,0.15,1.0 };
	GLfloat mat_speclar[] = { 0.8,0.8,0.8,1.0 };
	GLfloat mat_shiness[] = { 96.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, speclar);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_speclar);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

}

void QuatApp::UpdatePosition(cyclone::Quaternion *deltQuat, cyclone::Quaternion *crntQuat,
	cyclone::Vector3 *parntPos, cyclone::Vector3 *this_pos,
	cyclone::Vector3 *that_pos, OBJECT_NAME obj_name)
{
	cyclone::Quaternion rot_quat = *deltQuat;
	rot_quat *= (*crntQuat);
	*crntQuat = rot_quat;
	crntQuat->normalise();

	cyclone::Matrix4 matrix34;

	if (obj_name == OBJ_SUN_MARK || obj_name == OBJ_EARTH)
	{
		matrix34.setOrientationAndPos(*crntQuat, *parntPos);
		*that_pos = matrix34.transform(*this_pos);
	}
	else if (obj_name == OBJ_EARTH_MARK || obj_name == OBJ_MOON)
	{
		rot_quat = (*crntQuat);
		rot_quat *= e2sQuatA;
		rot_quat.normalise();
		matrix34.setOrientationAndPos(rot_quat, *parntPos);
		*that_pos = matrix34.transform(*this_pos);

		m2e_axis = matrix34.transformDirection(M2E_AXIS);

		if (obj_name == OBJ_EARTH_MARK)
		{
			e_LnTips[0] = matrix34.transform(eLN_TIPS[0]);
			e_LnTips[1] = matrix34.transform(eLN_TIPS[1]);
			e_LnTips[2] = matrix34.transform(eLN_TIPS[2]);
		}
	}
	else if (obj_name == OBJ_MOON_MARK)
	{
		rot_quat = (*crntQuat);
		rot_quat *= m2eQuatA;
		rot_quat *= e2sQuatA;
		rot_quat.normalise();
		matrix34.setOrientationAndPos(rot_quat, *parntPos);
		*that_pos = matrix34.transform(*this_pos);

		m_LnTips[0] = matrix34.transform(mLN_TIPS[0]);
		m_LnTips[1] = matrix34.transform(mLN_TIPS[1]);
		m_LnTips[2] = matrix34.transform(mLN_TIPS[2]);
	}
}

void QuatApp::DrawSphere(cyclone::Vector3 &sferePos, cyclone::Vector3 &sfereClr, float sfereSiz, int sfereRes)
{
	glPushMatrix();
	glTranslatef(sferePos[0], sferePos[1], sferePos[2]);
	glColor3f(sfereClr[0], sfereClr[1], sfereClr[2]);
	glutSolidSphere(sfereSiz, sfereRes, sfereRes);
	glPopMatrix();
}

void QuatApp::DrawMark(cyclone::Vector3 &mark_pos, float mark_rad, int mark_res, cyclone::Vector3 &mark_clr)
{
	glPushMatrix();
	glTranslatef(mark_pos[0], mark_pos[1], mark_pos[2]);
	glColor3f(mark_clr.x, mark_clr.y, mark_clr.z);
	glutSolidSphere(mark_rad, mark_res, mark_res);
	glPopMatrix();
}

void QuatApp::DrawSystemAxes(cyclone::Vector3 *originPt, cyclone::Vector3 *arwHeads)
{
	glBegin(GL_LINES);
}

void QuatApp::sunView()
{
	user_eye[0] = 0.0;
	user_eye[1] = user_hgt;
	user_eye[2] = 25.0;
	focusPnt[0] = 0.0;
	focusPnt[1] = 0.0;
	focusPnt[2] = 0.0;
	upVector[0] = 0.0;
	upVector[1] = 1.0;
	upVector[2] = 0.0;
}

void QuatApp::EarthRadialView()
{
	float e2e_dist = eth_size * 40.0;
	focusPnt[0] = earthPos[0];
	focusPnt[1] = earthPos[1];
	focusPnt[2] = earthPos[2];
	cyclone::Vector3 sunTOeth = earthPos - sun_cord;
	sunTOeth.normalise();
	cyclone::Vector3 ethTOeye = sunTOeth * e2e_dist;
	user_eye[0] = earthPos[0] + ethTOeye[0];
	user_eye[1] = earthPos[1] + ethTOeye[1];
	user_eye[2] = earthPos[2] + ethTOeye[2];
	upVector[0] = 0.0;  upVector[1] = 1.0;  upVector[2] = 0.0;
}

void QuatApp::EarthTangentView()
{
	cyclone::Vector3 ethTOsun = sun_cord - earthPos;
	cyclone::Vector3 UpVector = cyclone::Vector3(0.0, 1.0, 0.0);
	cyclone::Vector3 negtvVec = UpVector % ethTOsun;
	negtvVec.normalise();
	negtvVec *= eth_size * 25.0;
	focusPnt[0] = earthPos[0];
	focusPnt[1] = earthPos[1];
	focusPnt[2] = earthPos[2];
	user_eye[0] = earthPos[0] + negtvVec[0];
	user_eye[1] = earthPos[1] + negtvVec[1];
	user_eye[2] = earthPos[2] + negtvVec[2];
	upVector[0] = 0.0;  upVector[1] = 1.0;  upVector[2] = 0.0;
}

void QuatApp::EarthTopDownView()
{
	cyclone::Vector3 ethTOsun = sun_cord - earthPos;
	cyclone::Vector3 up_vectr = cyclone::Vector3(0.0, 1.0, 0.0);
	cyclone::Vector3 negtvTan = up_vectr % ethTOsun;
	cyclone::Vector3 botom_up = ethTOsun % negtvTan;
	botom_up.normalise();
	botom_up *= eth_size * 20.0;
	focusPnt[0] = earthPos[0];
	focusPnt[1] = earthPos[1];
	focusPnt[2] = earthPos[2];
	user_eye[0] = earthPos[0] + botom_up[0];
	user_eye[1] = earthPos[1] + botom_up[1];
	user_eye[2] = earthPos[2] + botom_up[2];
	upVector[0] = -negtvTan[0];
	upVector[1] = -negtvTan[1];
	upVector[2] = -negtvTan[2];

}

void QuatApp::MoonRadialView()
{
	float m2e_dist = mun_size * 20.0;
	focusPnt[0] = moon_pos[0];
	focusPnt[1] = moon_pos[1];
	focusPnt[2] = moon_pos[2];
	cyclone::Vector3 ethTOMun = moon_pos - earthPos;
	ethTOMun.normalise();
	cyclone::Vector3 munTOeye = ethTOMun * m2e_dist;
	user_eye[0] = moon_pos[0] + munTOeye[0];
	user_eye[1] = moon_pos[1] + munTOeye[1];
	user_eye[2] = moon_pos[2] + munTOeye[2];
	upVector[0] = m2e_axis[0];
	upVector[1] = m2e_axis[1];
	upVector[2] = m2e_axis[2];
}

void QuatApp::MoonTangentView()
{
	float m2e_dist = mun_size * 30.0;
	focusPnt[0] = moon_pos[0];
	focusPnt[1] = moon_pos[1];
	focusPnt[2] = moon_pos[2];
	cyclone::Vector3 ethTOMun = moon_pos - earthPos;
	ethTOMun.normalise();
	cyclone::Vector3 munTOeye = ethTOMun % m2e_axis;
	munTOeye.normalise();
	munTOeye *= m2e_dist;
	user_eye[0] = moon_pos[0] + munTOeye[0];
	user_eye[1] = moon_pos[1] + munTOeye[1];
	user_eye[2] = moon_pos[2] + munTOeye[2];
	upVector[0] = m2e_axis[0]; upVector[1] = m2e_axis[1]; upVector[2] = m2e_axis[2];

}

void QuatApp::MoonTopDownView()
{
	float m2e_dist = mun_size * 55.0;
	focusPnt[0] = moon_pos[0];
	focusPnt[1] = moon_pos[1];
	focusPnt[2] = moon_pos[2];
	cyclone::Vector3 ethTOMun = moon_pos - earthPos;
	cyclone::Vector3 tanVectr = m2e_axis % ethTOMun;
	tanVectr.normalise();
	cyclone::Vector3 moon2Eye = m2e_axis * m2e_dist;
	user_eye[0] = moon_pos[0] + moon2Eye[0];
	user_eye[1] = moon_pos[1] + moon2Eye[1];
	user_eye[2] = moon_pos[2] + moon2Eye[2];
	upVector[0] = tanVectr[0]; upVector[1] = tanVectr[1]; upVector[2] = tanVectr[2];
}

void QuatApp::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	//view mode
	switch (viewMode)
	{
		case 1: sunView(); break;
		case 2: EarthRadialView(); break;
		case 3: EarthTangentView(); break;
		case 4: EarthTopDownView(); break;
		case 5: MoonRadialView(); break;
		case 6: MoonTangentView(); break;
		case 7: MoonTopDownView(); break;
	}

	glLoadIdentity();
	gluLookAt(user_eye[0], user_eye[1], user_eye[2],
			  focusPnt[0], focusPnt[1], focusPnt[2],
			  upVector[0], upVector[1], upVector[2]);

	// draw 3 axis-lines to represent EACH system and draw the inter-planet connections
	glDisable(GL_LIGHTING);
	if (sun_axes) DrawSystemAxes(&sun_cord, sLN_TIPS);
	if (eth_axes) DrawSystemAxes(&earthPos, e_LnTips);
	if (mun_axes) DrawSystemAxes(&moon_pos, m_LnTips);
	if (connexon) DrawConnectionLines();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// draw the sun and the mark
	DrawSphere(sun_cord, sunColor, sun_size, 20);
	DrawMark(sun_mark, sRefSize, 10, s_mColor);

	// draw the earth and the mark
	DrawSphere(earthPos, earthClr, eth_size, 16);
	DrawMark(eth_mark, eRefSize, 10, e_mColor);

	// draw the moon and the mark
	DrawSphere(moon_pos, moon_clr, mun_size, 16);
	DrawMark(moonMark, mRefSize, 10, m_mColor);

	// Show some info on the screen
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);

	static char modeStrs[4][3][20] =
	{
		{"Q-Yaw", "W-Roll", "E-Pitch"},
	    {"A-Yaw", "S-Roll", "D-Pitch"},
	    {"Z-Yaw", "X-Roll", "C-Pitch"},
	    {"B-Yaw", "N-Roll", "M-Pitch"}
	};

	static char viewStrs[7][100] =
	{
		"1: Sun view", "2: Earth Radial View",
		"3: Earth Tangent view", "4: Earth Top-Down view",
		"5: Moon radial View", "6: Moon Tangent View",
		"7: Moon Top-Down view"
	};

	// rotation mode of EACH object
	renderText(60, height - 20, "Sun (Self Rotation: Q-Yaw / W-Roll / E-Pitch)");
	glColor3f(1.0, 1.0, 0.0);
	renderText(60 + 340, height - 20, modeStrs[0][sun_mode]); glColor3f(1.0, 1.0, 1.0);

	renderText(60, height - 40, "Earth (Self Rotation: A-Yaw / S-Roll / D-Pitch)");
	glColor3f(1.0, 1.0, 0.0);
	renderText(60 + 340, height - 40, modeStrs[1][sun_mode]); glColor3f(1.0, 1.0, 1.0);

	renderText(60, height - 60, "Moon (Self Rotation: Z-Yaw / X-Roll / C-Pitch)");
	glColor3f(1.0, 1.0, 0.0);
	renderText(60 + 340, height - 60, modeStrs[2][sun_mode]); glColor3f(1.0, 1.0, 1.0);

	renderText(60, height - 80, "Moon (Self Rotation: B-Yaw / N-Roll / M-Pitch)");
	glColor3f(1.0, 1.0, 0.0);
	renderText(60 + 340, height - 80, modeStrs[3][sun_mode]); glColor3f(1.0, 1.0, 1.0);

	// view modes
	renderText(60, height - 120, "1: Sun View");
	renderText(60, height - 140, "2: Earth Radial View");
	renderText(60, height - 160, "3: Earth Tangent View");
	renderText(60, height - 180, "4: Earth Top-Down View");
	renderText(60, height - 200, "5: Moon radial View");
	renderText(60, height - 220, "6: Moon Tangent View");
	renderText(60, height - 240, "7: Moon Top-Down view");

	glColor3f(1.0, 1.0, 0.0);
	renderText(60, height - 260, "Current view mode");
	renderText(60 + 340, height - 260, viewStrs[viewMode-1]);
	glColor3f(1.0, 1.0, 1.0);
}

void QuatApp::updateObjects(cyclone::real duration)
{
	UpdatePosition(&s_self_q, &sun_quat, &sun_cord, &SUN_MARK, &sun_mark, OBJ_SUN_MARK);
	UpdatePosition(&e2sQuatB, &e2sQuatA, &sun_cord, &earthPOS, &earthPos, OBJ_EARTH);
	UpdatePosition(&eSELF_qB, &eSELF_qA, &earthPos, &ETH_MARK, &eth_mark, OBJ_EARTH_MARK);
	UpdatePosition(&m2eQuatB, &m2eQuatA, &earthPos, &moon_POS, &moon_pos, OBJ_MOON);
	UpdatePosition(&mSELF_qB, &mSELF_qA, &moon_pos, &moonMARK, &moonMark, OBJ_MOON_MARK);
}

void QuatApp::key(unsigned char key)
{
	switch (key)
	{
	case 'r': case 'R': ApplyDefaultContext(); break;
	case '-': case '_': if (theScale > 1)
	{
		theScale--;
		speedAdj = SPEEDADJ * theScale;
		ApplyAngularVelocity();
	} break;
	case '+': case '=': if (theScale < 128)
	{
		theScale++;
		speedAdj = SPEEDADJ * theScale;
		ApplyAngularVelocity();
	} break;
	// set the self-rotation mode for the sun
	case 'q': case 'Q': Sun_SelfRotation_Yaw(); break;
	case 'w': case 'W': Sun_SelfRotation_Roll(); break;
	case 'e': case 'E': Sun_SelfRotation_Pitch(); break;

	case 'a': case 'A': Earth_SelfRotation_Yaw(); break;
	case 's': case 'S': Earth_SelfRotation_Roll(); break;
	case 'd': case 'D': Earth_SelfRotation_Pitch(); break;

	case 'z': case 'Z': Moon_SelfRotation_Yaw(); break;
	case 'x': case 'X': Moon_SelfRotation_Roll(); break;
	case 'c': case 'C': Moon_SelfRotation_Pitch(); break;

	case 'b': case 'B': Moon_AroundEarth_Yaw(); break;
	case 'n': case 'N': Moon_AroundEarth_Roll(); break;
	case 'm': case 'M': Moon_AroundEarth_Pitch(); break;

	case '<': case ',': if ((viewMode == 1) && (user_hgt >= 1.0)) user_hgt -= 1.0; break;
	case '>': case '.': if ((viewMode == 1) && (user_hgt < USER_HGT)) user_hgt += 1.0; break;


	// toggle on/off axes and connection lines
	case 'h': case 'H': sun_axes = 1 - sun_axes; break;
	case 'j': case 'J': eth_axes = 1 - eth_axes; break;
	case 'k': case 'K': mun_axes = 1 - mun_axes; break;
	case 'l': case 'L': connexon = 1 - connexon; break;

		// set the view mode for the user
	case '1': viewMode = 1; break;
	case '2': viewMode = 2; break;
	case '3': viewMode = 3; break;
	case '4': viewMode = 4; break;
	case '5': viewMode = 5; break;
	case '6': viewMode = 6; break;
	case '7': viewMode = 7; break;


	}
}

void QuatApp::ApplyDefaultContext()
{
	reset();
	pauseSimulation = false;
	ApplyAngularVelocity();
}

void QuatApp::reset()
{
	// view settings
	user_hgt = USER_HGT;
	user_eye[0] = 0.00; user_eye[1] = user_hgt; user_eye[2] = 25.0;
	focusPnt[0] = 0.00; focusPnt[1] = 0.00; focusPnt[2] = 0.00;
	upVector[0] = 0.00; upVector[1] = 1.00; upVector[2] = 0.00;

	// the sun
	sun_mode = 1;  // roll
	sun_cord = cyclone::Vector3(0.0, 0.0, 0.0);
	sun_axis = cyclone::Vector3(0.0, 0.0, 1.0);
	SUN_MARK = cyclone::Vector3(sun_size, 0.0, 0.0);
	sun_mark = SUN_MARK;
	sun_quat = cyclone::Quaternion();
	s_self_q = cyclone::Quaternion();

	// the earth
	eth_mode = 2;  // pitch
	earthPOS = cyclone::Vector3(ethTrack, 0.0, 0.0);
	earthPos = earthPOS;
	earthAOR = cyclone::Vector3(1.0, 0.0, 0.0);
	ETH_MARK = cyclone::Vector3(0.0, 0.0, eth_size);
	eth_mark = ETH_MARK;
	eSELF_qA = cyclone::Quaternion();
	eSELF_qB = cyclone::Quaternion();

	e2s_axis = cyclone::Vector3(0.0, 1.0, 0.0);
	e2sQuatA = cyclone::Quaternion();
	e2sQuatB = cyclone::Quaternion();

	// the moon
	munMode0 = 0;  // yaw ... self_rotation
	moon_POS = cyclone::Vector3(munTrack, 0.0, 0.0);
	moon_pos = moon_POS;
	moonAxis = cyclone::Vector3(0.0, 1.0, 0.0);
	moonMARK = cyclone::Vector3(mun_size, 0.0, 0.0);
	moonMark = moonMARK;
	mSELF_qA = cyclone::Quaternion();
	mSELF_qB = cyclone::Quaternion();

	munMode1 = 1;  // roll ... self_revolution
	M2E_AXIS = cyclone::Vector3(0.0, 1.0, 0.0);
	m2e_axis = cyclone::Vector3(0.0, 1.0, 0.0);
	m2eQuatA = cyclone::Quaternion();
	m2eQuatB = cyclone::Quaternion();
}

void QuatApp::initGraphics()
{
	SetLightMaterial(-10.0, 0.0, 25.0);
	Application::initGraphics();
}

void QuatApp::DrawConnectionLines()
{
	glLineStipple(1, 0xAAAA);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);

	glVertex3f(sun_cord.x, sun_cord.y, sun_cord.z);
	glVertex3f(earthPos.x, earthPos.y, earthPos.z);

	glVertex3f(earthPos.x, earthPos.y, earthPos.z);
	glVertex3f(moon_pos.x, moon_pos.y, moon_pos.z);
	glEnd();

	glEnable(GL_LINE_STIPPLE);
}

const char* QuatApp::getTitle()
{
	return "QuatApp";
}

Application* getApplication()
{
	return new QuatApp();
}