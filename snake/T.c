//#include <stdio.h>
//
//int score, best_score;
//
//void writeRecord()
//{
//	FILE* fp;
//	if (fp != NULL)
//	{
//		fopen_s(&fp, "BestScore", "w");
//		fprintf_s(fp, "%d", score);
//	}
//	fclose(fp);
//}
//
////游戏历史最高分读取
//void readRecord()
//{
//	FILE* fp;
//	fopen_s(&fp, "BestScore", "r");
//	if (fp != NULL)
//	{
//		fscanf_s(fp, "%d", &best_score);
//	}
//	fclose(fp);
//}
//
//int main01()
//{
//
//
//}