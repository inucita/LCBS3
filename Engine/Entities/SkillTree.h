#ifndef __SKILL_TREE_H__
#define __SKILL_TREE_H__

#define DEF_SKILL_COL	(8)
struct stSkillTreeRow
{
	int index[DEF_SKILL_COL];

	stSkillTreeRow()
	{
		int i;
		for( i = 0; i < DEF_SKILL_COL; ++i )
			index[i] = -1;
	}
};

class CSkillTree
{
public:
	~CSkillTree();	

	static int LoadSkillTreeDataFromFile(const char* FileName);

	static stSkillTreeRow* m_SkillTree[9][3];
	static int m_nSkillCount[9][3];

	static void	Destroy();

private:
#ifdef G_THAI
	//----------------------------------------------->
	// skilltree.bin�� ������ ���뵥��Ÿ �Դϴ�.
	// �±��� ��� ť�꽺ų�� �߰����� �ʾƼ� �ڵ�󿡼� �����۾��� ���� �Ǿ����ϴ�.
	static void ChangeExclusionData();
	//-----------------------------------------------<
#endif // G_THAI

#ifdef VER_TEST	// ver_test�� ��쿡 �߸��� ����Ÿ ȭ�鿡 ����ϱ� ���� �뵵.
	static void ErrDataOutput();
#endif // VER_TEST
};

#endif // __SKILL_TREE_H__