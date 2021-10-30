#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

void progonka_X(const int i0, const int iN, int fiks, double** Subs, double tau, double* A, double* B, double* C, double* D);
void progonka_Y(const int i0, const int iN, int fiks, double** Subs, double tau, double* A, double* B, double* C, double* D);


int main()
{
	//Для вывода определенного числа цифорок
	cout << fixed;
	cout.precision(6);

	//Описание и содание файлов
	ofstream file_temperature;	file_temperature.open("temperature.dat");


	//Описнаие и инициализация переменных
	const int N(1000);           //Количество разбиений, N+1 количество точек
	int kk(1);                  //Счетчик, используется для вывода в файлов
	double
		shag_x,					//Пространственный шаг по R
		shag_y,					//Пространственный шаг по Z
		tau,					//Временной шаг
		time(0.0);				//Время

	//Функция тока
		double** teta = new double*[N + 1];						//температура
	double** teta_check = new double*[N + 1];

	for (int j = 0; j <= N; ++j) {
		teta[j] = new double[N + 1];
		teta_check[j] = new double[N + 1];
	}
	//Коэффициенты для прогонки
	double* A = new double[N + 1];
	double* B = new double[N + 1];
	double* C = new double[N + 1];
	double* D = new double[N + 1];

	shag_x = 1.0 / N;
	shag_y = 1.0 / N;
	tau = 0.01;

	//Задание начальных и граничных условий
	for (int j = 0; j <= N; ++j)
	{
		//Тут я просто задам  всю область опредеднной температуры и позже переопределю граничные точки
		for (int i = 0; i <= N; ++i)
		{
			teta[i][j] = 0.0;
		}
	}
		int
			x_topLeft(275),			x_topRight(725),			//Верхние левый и правый углы
			x_bottonLeft(0),		x_bottomRight(1000),		//Нижние левый и правый углы
			x_leftOffset(200),		x_rightOffset(800),			//Это координаты ножек
			x_leftMiddle(350),		x_rightMiddle(650),			//Координаты начала "выступа" буквы Д (шапки)


			y_start(0), y_end(1000),
			y_pallet(300),											//Координата "поддона"
			y_table(500),											//"Стол" буквы
			y_innerTop(925);										//Координата унтренней части верх


		for (int j = 1; j <= y_pallet; ++j) {
			for (int i = 1; i <= x_leftOffset - 1; ++i) {
				teta[i][j] = 0.0;
			}
		}

		for (int j = 1; j <= y_pallet; ++j) {
			for (int i = x_rightOffset + 1; i <= x_bottomRight - 1; ++i) {
				teta[i][j] = 0.0;
			}
		}

		for (int j = y_pallet + 1; j <= y_table - 1; ++j) {
			for (int i = 1; i <= x_bottomRight - 1; ++i) {
				teta[i][j] = 0.0;
			}
		}

		for (int j = y_table; j <= y_innerTop; ++j) {
			for (int i = x_topLeft + 1; i <= x_leftMiddle - 1; ++i) {
				teta[i][j] = 0.0;
			}
			
		}

		for (int j = y_table; j <= y_innerTop; ++j) {
			for (int i = x_rightMiddle + 1; i <= x_topRight - 1; ++i) {
				teta[i][j] = 0.0;
			}
		}

		for (int j = y_innerTop + 1; j <= y_end - 1; ++j) {
			for (int i = x_topLeft + 1; i <= x_topRight - 1; ++i) {
				teta[i][j] = 0.0;
			}
		}
		for (int i = 1; i <= x_leftOffset - 1; ++i) {
			for (int j = 1; j <= y_table - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_rightOffset + 1; i <= x_bottomRight - 1; ++i) {
			for (int j = 0 + 1; j <= y_table - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_leftOffset; i <= x_topLeft; ++i) {
			for (int j = y_pallet + 1; j <= y_table - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_topRight; i <= x_rightOffset; ++i) {
			for (int j = y_pallet + 1; j <= y_table - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_topLeft + 1; i <= x_leftMiddle - 1; ++i) {
			for (int j = y_pallet + 1; j <= y_end - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_rightMiddle + 1; i <= x_topRight - 1; ++i) {
			for (int j = y_pallet + 1; j <= y_end - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_leftMiddle; i <= x_rightMiddle; ++i) {
			for (int j = y_pallet + 1; j <= y_table - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}


		for (int i = x_leftMiddle; i <= x_rightMiddle; ++i) {
			for (int j = y_innerTop + 1; j <= y_end - 1; ++j) {
				teta[i][j] = 0.0;
			}
		}
		
			//Доопределение граничных точек

		/*	//Задание температуры для горячих ножек
	//	{
			for (int i = 0; i <= x_leftOffset; ++i) {
				teta[i][0] = 1.0;
			}
			for (int i = x_rightOffset; i <= x_bottomRight; ++i) {
				teta[i][0] = 1.0;
			}

			//Задание температуры для горячей "шапки"
			for (int i = x_topLeft; i <= x_topRight; ++i) {
				teta[i][y_end] = 1.0;
			}

			for (int j = 0; j <= y_table; j++) {
				teta[0][j] = 0.0;
				teta[x_bottomRight][j] = 0.0;
			}

			for (int j = 0; j <= y_pallet; j++) {
				teta[x_leftOffset][j] = 0.0;
				teta[x_rightOffset][j] = 0.0;
			}
			for (int i = x_leftOffset; i <= x_rightOffset; i++) {
				teta[i][y_pallet] = 0.0;
			}

			for (int i = 0; i <= x_topLeft; i++) {
				teta[i][y_table] = 0.0;
			}

			for (int i = x_topRight; i <= x_bottomRight; i++) {
				teta[i][y_table] = 0.0;
			}

			for (int j = y_table; j <= y_end; j++) {
				teta[x_topLeft][j] = 0.0;
				teta[x_topRight][j] = 0.0;
			}

			for (int i = x_topLeft; i <= x_topRight; i++) {
				teta[i][y_end] = 1.0;
			}

			for (int i = x_leftMiddle; i <= x_rightMiddle; i++) {
				teta[i][y_innerTop] = 0.0;
				teta[i][y_table] = 1.0;
			}

			for (int j = y_table; j <= y_innerTop; j++) {
				teta[x_leftMiddle][j] = 0.0;
				teta[x_rightMiddle][j] = 1.0;
			}
		}
		*/
		//Начало счета времени
		while (time <= 5000000000000000000)
		{
		{
			for (int j = 1; j <= y_pallet; j++) {
				for (int i = 1; i <= x_leftOffset - 1; ++i) {
					A[i] = C[i] = 1.0 / pow(shag_x, 2);
					B[i] = (-2.0) / pow(shag_x, 2) - (1.0) / tau;
					D[i] = -teta[i][j] / tau;
				}
				A[0] = 0.0; B[0] = 1.0; C[0] = 0.0; D[0] = 0.0;
				A[x_leftOffset] = 0.0; B[x_leftOffset] = 1.0; C[x_leftOffset] = 0.0; D[x_leftOffset] = 0.0;
				progonka_X(0, x_leftOffset, j, teta, tau, A, B, C, D);
			}

			for (int j = 1; j <= y_pallet; j++) {
				for (int i = x_rightOffset + 1; i <= x_bottomRight - 1; i++) {
					A[i] = C[i] = 1.0 / pow(shag_x, 2);
					B[i] = (-2.0) / pow(shag_x, 2) - (1.0) / tau;
					D[i] = -teta[i][j] / tau;
				}
				A[x_rightOffset] = 0.0; B[x_rightOffset] = 1.0; C[x_rightOffset] = 0.0; D[x_rightOffset] = 0.0;
				A[x_bottomRight] = 0.0; B[x_bottomRight] = 1.0; C[x_bottomRight] = 0.0; D[x_bottomRight] = 0.0;
				progonka_X(x_rightOffset, x_bottomRight, j, teta, tau, A, B, C, D);
			}

			for (int j = y_pallet + 1; j <= y_table - 1; j++) {
				for (int i = 1; i <= x_bottomRight - 1; i++) {
					A[i] = C[i] = 1.0 / pow(shag_x, 2);
					B[i] = (-2.0) / pow(shag_x, 2) - (1.0) / tau;
					D[i] = -teta[i][j] / tau;
				}
				A[0] = 0.0; B[0] = 1.0; C[0] = 0.0; D[0] = 0.0;
				A[x_bottomRight] = 0.0; B[x_bottomRight] = 1.0; C[x_bottomRight] = 0.0; D[x_bottomRight] = 0.0;
				progonka_X(0, x_bottomRight, j, teta, tau, A, B, C, D);
			}

			for (int j = y_table; j <= y_innerTop; j++) {
				for (int i = x_topLeft + 1; i <= x_leftMiddle - 1; i++) {
					A[i] = C[i] = 1.0 / pow(shag_x, 2);
					B[i] = (-2.0) / pow(shag_x, 2) - (1.0) / tau;
					D[i] = -teta[i][j] / tau;
				}
				A[x_topLeft] = 0.0; B[x_topLeft] = 1.0; C[x_topLeft] = 0.0; D[x_topLeft] = 0.0;
				A[x_leftMiddle] = 0.0; B[x_leftMiddle] = 1.0; C[x_leftMiddle] = 0.0; D[x_leftMiddle] = 0.0;
				progonka_X(x_topLeft, x_leftMiddle, j, teta, tau, A, B, C, D);
			}

			for (int j = y_table; j <= y_innerTop; j++) {
				for (int i = x_rightMiddle + 1; i <= x_topRight - 1; i++) {
					A[i] = C[i] = 1.0 / pow(shag_x, 2);
					B[i] = (-2.0) / pow(shag_x, 2) - (1.0) / tau;
					D[i] = -teta[i][j] / tau;
				}
				A[x_rightMiddle] = 0.0; B[x_rightMiddle] = 1.0; C[x_rightMiddle] = 0.0; D[x_rightMiddle] = 0.0;
				A[x_topRight] = 0.0; B[x_topRight] = 1.0; C[x_topRight] = 0.0; D[x_topRight] = 0.0;
				progonka_X(x_rightMiddle, x_topRight, j, teta, tau, A, B, C, D);
			}

			for (int j = y_innerTop + 1; j <= y_end - 1; j++) {
				for (int i = x_topLeft + 1; i <= x_topRight - 1; i++) {
					A[i] = C[i] = 1.0 / pow(shag_x, 2);
					B[i] = (-2.0) / pow(shag_x, 2) - (1.0) / tau;
					D[i] = -teta[i][j] / tau;
				}
				A[x_topLeft] = 0.0; B[x_topLeft] = 1.0; C[x_topLeft] = 0.0; D[x_topLeft] = 0.0;
				A[x_topRight] = 0.0; B[x_topRight] = 1.0; C[x_topRight] = 0.0; D[x_topRight] = 0.0;
				progonka_X(x_topLeft, x_topRight, j, teta, tau, A, B, C, D);
			}
		}
			/* -----------------------------------------------------------------------------------------------------------------------*/

			/* -----------------------------------------------------------------------------------------------------------------------*/
			/* -----------------------------------------------------------------------------------------------------------------------*/
			/* -----------------------------------------------------------------------------------------------------------------------*/
			/* -----------------------------------------------------------------------------------------------------------------------*/
			//тут Начинается прогонка областей по Y
		{
			for (int i = 1; i <= x_leftOffset - 1; i++) {
				for (int j = 1; j <= y_table - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[0] = 0.0; B[0] = 1.0; C[0] = 0.0; D[0] = 1.0;
				A[y_table] = 0.0; B[y_table] = 1.0; C[y_table] = 0.0; D[y_table] = 0.0;
				progonka_Y(0, y_table, i, teta, tau, A, B, C, D);
			}


			for (int i = x_rightOffset + 1; i <= x_bottomRight - 1; i++) {
				for (int j = 0 + 1; j <= y_table - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[0] = 0.0; B[0] = 1.0; C[0] = 0.0; D[0] = 1.0;
				A[y_table] = 0.0; B[y_table] = 1.0; C[y_table] = 0.0; D[y_table] = 0.0;
				int N_rax = y_table - 0;
				progonka_Y(0, y_table, i, teta, tau, A, B, C, D);
			}


			for (int i = x_leftOffset; i <= x_topLeft; i++) {
				for (int j = y_pallet + 1; j <= y_table - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[y_pallet] = 0.0; B[y_pallet] = 1.0; C[y_pallet] = 0.0; D[y_pallet] = 0.0;
				A[y_table] = 0.0; B[y_table] = 1.0; C[y_table] = 0.0; D[y_table] = 0.0;
				progonka_Y(y_pallet, y_table, i, teta, tau, A, B, C, D);
			}


			for (int i = x_topRight; i <= x_rightOffset; i++) {
				for (int j = y_pallet + 1; j <= y_table - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[y_pallet] = 0.0; B[y_pallet] = 1.0; C[y_pallet] = 0.0; D[y_pallet] = 0.0;
				A[y_table] = 0.0; B[y_table] = 1.0; C[y_table] = 0.0; D[y_table] = 0.0;
				int N_rax = y_table - y_pallet;
				progonka_Y(y_pallet, y_table, i, teta, tau, A, B, C, D);
			}


			for (int i = x_topLeft + 1; i <= x_leftMiddle - 1; i++) {
				for (int j = y_pallet + 1; j <= y_end - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[y_pallet] = 0.0; B[y_pallet] = 1.0; C[y_pallet] = 0.0; D[y_pallet] = 0.0;
				A[y_end] = 0.0; B[y_end] = 1.0; C[y_end] = 0.0; D[y_end] = 1.0;
				int N_rax = y_end - y_pallet;
				progonka_Y(y_pallet, y_end, i, teta, tau, A, B, C, D);
			}


			for (int i = x_rightMiddle + 1; i <= x_topRight - 1; i++) {
				for (int j = y_pallet + 1; j <= y_end - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[y_pallet] = 0.0; B[y_pallet] = 1.0; C[y_pallet] = 0.0; D[y_pallet] = 0.0;
				A[y_end] = 0.0; B[y_end] = 1.0; C[y_end] = 0.0; D[y_end] = 1.0;
				progonka_Y(y_pallet, y_end, i, teta, tau, A, B, C, D);
			}


			for (int i = x_leftMiddle; i <= x_rightMiddle; i++) {
				for (int j = y_pallet + 1; j <= y_table - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[y_pallet] = 0.0; B[y_pallet] = 1.0; C[y_pallet] = 0.0; D[y_pallet] = 0.0;
				A[y_table] = 0.0; B[y_table] = 1.0; C[y_table] = 0.0; D[y_table] = 1.0;
				progonka_Y(y_pallet, y_table, i, teta, tau, A, B, C, D);
			}


			for (int i = x_leftMiddle; i <= x_rightMiddle; i++) {
				for (int j = y_innerTop + 1; j <= y_end - 1; j++) {
					A[j] = C[j] = 1.0 / pow(shag_y, 2);
					B[j] = (-2.0) / pow(shag_y, 2) - (1.0) / tau;
					D[j] = -teta[i][j] / tau;
				}
				A[y_innerTop] = 0.0; B[y_innerTop] = 1.0; C[y_innerTop] = 0.0; D[y_innerTop] = 0.0;
				A[y_end] = 0.0; B[y_end] = 1.0; C[y_end] = 0.0; D[y_end] = 1.0;
				progonka_Y(y_innerTop, y_end, i, teta, tau, A, B, C, D);
			}

		}
			//Локальные граничные условия
			for (int i = 0; i <= x_leftOffset; ++i) {
				teta[i][0] = 1.0;
			}

			for (int i = x_rightOffset; i <= x_bottomRight; ++i) {
				teta[i][0] = 1.0;
			}

			for (int i = x_topLeft; i <= x_topRight; ++i) {
				teta[i][N] = 1.0;
			}
			
			//Вывод значений в файл	
			if (time >= kk * 20.0) {
				for (int i = 0; i <= N; ++i) {
					for (int j = 0; j <= N; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}
				
				
			/*
				//--------------------------------------------------------------------------------------------------------------------------------
				//--------------------------------------------------------------------------------------------------------------------------------
				{
					for (int j = 0; j <= y_pallet - 1; ++j) {
						for (int i = 0; i <= x_leftOffset; ++i) {
							file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
						}
						for (int i = x_rightOffset; i <= x_bottomRight; ++i) {
							file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
						}
					}

					for (int j = y_pallet; j <= y_table; ++j) {
						for (int i = 0; i <= x_bottomRight; ++i) {
							file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
						}
					}

					for (int j = y_table + 1; j <= y_innerTop - 1; ++j) {
						for (int i = x_topLeft; i <= x_leftMiddle; ++i) {
							file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
						}
						for (int i = x_rightMiddle; i <= x_topRight; ++i) {
							file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
						}
					}
					for (int j = y_innerTop; j <= y_end; ++j) {
						for (int i = x_topLeft; i <= x_topRight; ++i) {
							file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
						}
					}
				}
				
				//for (int j = y_table; j <= y_innerTop; ++j) {
				//	for (int i = x_rightMiddle; i <= x_topRight; ++i) {
				//		file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
				//	}
				//}

				
				for (int i = 0; i <= x_leftOffset; ++i) {
					for (int j = 0; j <= y_table; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_rightOffset; i <= x_bottomRight; ++i) {
					for (int j = 0; j <= y_table; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_leftOffset; i <= x_topLeft; ++i) {
					for (int j = y_pallet; j <= y_table; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_topRight; i <= x_rightOffset; ++i) {
					for (int j = y_pallet; j <= y_table; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_topLeft; i <= x_leftMiddle; ++i) {
					for (int j = y_pallet; j <= y_end; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_rightMiddle; i <= x_topRight; ++i) {
					for (int j = y_pallet; j <= y_end; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_leftMiddle; i <= x_rightMiddle; ++i) {
					for (int j = y_pallet; j <= y_table; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					}
				}


				for (int i = x_leftMiddle; i <= x_rightMiddle; ++i) {
					for (int j = y_innerTop; j <= y_end; ++j) {
						file_temperature << i * shag_x << "   " << j * shag_y << "   " << teta[i][j] << endl;
					} 
				}
				*/
				//--------------------------------------------------------------------------------------------------------------------------------
				//--------------------------------------------------------------------------------------------------------------------------------
				
				cout << kk *5.0 << endl;
				kk += 1;
			}


			time += tau;
		}
}

//Это функция прогонки по координате X
void progonka_X(const int i0, const int iN, int fiks, double** Subs, double tau, double* A, double* B, double* C, double* D)	//fiks - фиксированное значение строки(столбца)
{
	double* prog_koef_alfa = new double[iN + 1];
	double* prog_koef_beta = new double[iN + 1];

	prog_koef_alfa[i0] = (-1.0) * C[i0] / B[i0];
	prog_koef_beta[i0] = D[i0] / B[i0];

	for (int i = i0 + 1; i < iN; ++i)
	{
		prog_koef_alfa[i] = (-1.0) * C[i] / (A[i] * prog_koef_alfa[i - 1] + B[i]);
		prog_koef_beta[i] = (D[i] - A[i] * prog_koef_beta[i - 1]) / (A[i] * prog_koef_alfa[i - 1] + B[i]);
	}

	Subs[iN][fiks] = (D[iN] - A[iN] * prog_koef_beta[iN - 1]) / (A[iN] * prog_koef_alfa[iN - 1] + B[iN]);

	for (int i = iN - 1; i >= i0; --i)
		Subs[i][fiks] = Subs[i + 1][fiks] * prog_koef_alfa[i] + prog_koef_beta[i];

	delete[] prog_koef_alfa;
	delete[] prog_koef_beta;
}



//Это функция прогонки по координате Y
void progonka_Y(const int i0, const int iN, int fiks, double** Subs, double tau, double* A, double* B, double* C, double* D)
{
	double* prog_koef_alfa = new double[iN + 1];
	double* prog_koef_beta = new double[iN + 1];

	prog_koef_alfa[i0] = (-1.0) * C[i0] / B[i0];
	prog_koef_beta[i0] = D[i0] / B[i0];

	for (int j = i0 + 1; j < iN; ++j)
	{
		prog_koef_alfa[j] = (-1.0) * C[j] / (A[j] * prog_koef_alfa[j - 1] + B[j]);
		prog_koef_beta[j] = (D[j] - A[j] * prog_koef_beta[j - 1]) / (A[j] * prog_koef_alfa[j - 1] + B[j]);
	}

	Subs[fiks][iN] = (D[iN] - A[iN] * prog_koef_beta[iN - 1]) / (A[iN] * prog_koef_alfa[iN - 1] + B[iN]);

	for (int j = iN - 1; j >= i0; --j)
		Subs[fiks][j] = Subs[fiks][j + 1] * prog_koef_alfa[j] + prog_koef_beta[j];

	delete[] prog_koef_alfa;
	delete[] prog_koef_beta;
}