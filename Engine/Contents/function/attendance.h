#ifndef		ATTENDANCE_H_
#define		ATTENDANCE_H_

class Attendance
{
public:
	Attendance();

	void RecvAttendanceMsg(CNetworkMessage* istr); // �⼮ üũ

	int GetAccDate() { return m_nAccDate; }
private:
	int m_nAccDate;
};


#endif		// ATTENDANCE_H_