/*=============================================================================
 |
 |       Author:  William Kozma Jr
 |                wkozma@ntia.gov
 |                US Dept of Commerce, NTIA/ITS
 |                June 2021 : Geneva Study Group 3 Meetings
 |
 *===========================================================================*/

class LOSTable
{
public:
	void Build(struct Path path, struct Terminal terminal_1, struct Terminal terminal_2, double lambda);
	double GetPsiFromTable(double d__km);

	LOSTable();
	~LOSTable();

private:
	void ThreeColumnMergeSort(double A_psi[], double A_delta_r[], double A_d[], int i_A,
		double B_psi[], double B_delta_r[], double B_d[], int i_B,
		double C_psi[], double C_delta_r[], double C_d[], int *i_C);
};

