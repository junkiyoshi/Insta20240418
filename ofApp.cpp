#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);
	ofSetColor(139);

	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();

	float phi_deg_step = 0.25;
	float theta_deg_step = 0.25;

	float R = 230;
	float r = R * 0.3;

	ofColor color;
	for (float threshold_base = 0; threshold_base <= 1.0; threshold_base += 0.05)
	{
		float threshold_1 = threshold_base - 0.0025;
		float threshold_2 = threshold_base + 0.0025;

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				auto noise_location = this->make_point(R, r, theta_deg, phi_deg) * 0.0005;
				auto noise_value = ofNoise(noise_location.x, noise_location.y, noise_location.z, ofGetFrameNum() * 0.01);
				if (noise_value <= threshold_1 || noise_value >= threshold_2) { continue; }

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg - phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg - theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));
				vertices.push_back(glm::vec3(this->make_point(R, r, theta_deg + theta_deg_step * 0.5, phi_deg + phi_deg_step * 0.5)));

				for (auto& vertex : vertices) {
					
					vertex.z = 0;
				}

				this->face.addVertices(vertices);

				color.setHsb(ofMap(threshold_base, 0, 1, 0, 255), 200, 255);
				this->face.addColor(color); this->face.addColor(color);
				this->face.addColor(color);	this->face.addColor(color);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);
			}
		}
}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->face.draw();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v, float scale) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u) * scale) * cos(v);
	auto y = (R + r * cos(u) * scale) * sin(v);
	auto z = r * sin(u) * scale;

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}