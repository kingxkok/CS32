class File {
private:
	string m_name;

public:
	File(string name);
	virtual ~File();
	string name() const;
	virtual void open() const = 0;
	virtual void redisplay() const;
	
};

File::File(string name) {
	m_name = name;
}

File::~File() {

}

string File::name() const {
	return m_name;
}

void File::redisplay() const {
	cout << "refresh the screen";
}




class  TextMsg :public File{
public:
	TextMsg(string name);
	virtual ~TextMsg();
	virtual void open() const;
};

TextMsg::TextMsg(string name) : File(name) {
	
}

TextMsg::~TextMsg() {
	cout << "Destroying " << name() << ", a text message" << endl;
}

void TextMsg::open() const {
	cout << "open text message";
}



class  Video : public File {
private: 
	int m_running_time;
public:
	Video(string name, int running_time);
	virtual ~Video();
	virtual void open() const;
	virtual void redisplay() const;

};

Video::Video(string name, int running_time) : File(name){
	m_running_time = running_time;
}

Video::~Video() {
	cout << "Destorying " << name() << ", a video" << endl;
}

void Video::open() const {
	cout << "play " << m_running_time << " second video";
}

void Video::redisplay() const {
	cout << "replay video";
}


class Picture : public File {
public:
	Picture(string name);
	virtual ~Picture();
	virtual void open() const;
};

Picture::Picture(string name) : File(name) {}
Picture::~Picture() {
	cout << "Destroying the picture " << name() << endl;
}
void Picture::open() const {
	cout << "show picture";
}