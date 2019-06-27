#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <functional>
#include <math.h>
#include <locale.h>
#include <Windows.h>
#include "helper.cpp"



using namespace sf;

int Height = 900;
int Width = 1600;
RenderWindow window(VideoMode(Width, Height), "Graph Calculator", Style::Titlebar | Style::Close);
RenderWindow windowDialog;

tgui::Theme::Ptr theme = tgui::Theme::create("../widgets/Black.txt");
tgui::Gui gui{ window };
tgui::Gui guiMainMenu{ window };
tgui::Gui guiInformer{ window };
tgui::Gui guiAboutProgram{ window };
tgui::Tab::Ptr tabs = tgui::Tab::create();

std::vector<int> Way_Dijkstra_Algorithm;
std::vector<int> Vertexs_Degree;
std::vector<int> CentralOrPeripheral;

Sprite vertexSprite;
Sprite selectedVertexSprite;
Texture simpleVertexTexture;
Texture selectedVertexTexture;
Texture edgeTexture;
Texture arrowTexture;
Texture edgeLoopTexture;
Sprite edgeLoopSprite;
Sprite edgeSprite;
Sprite arrowSprite;

Texture MenuTexture;

Font font;
Font edgeFont;

Text EdgeWeight;
Text VertexDegree;
Text CentOrPer;
Texture ButtonTexture;

bool MainField_isScrolling = false;

RectangleShape line;

bool MouseIsClicked = false;
int counter = 0;
int counterE = 0;
int NameOfVertex = 1;
int NameOfEdge = 1;
int SelectedVertex = 0;

std::vector <MyVertex> Vertexs;
std::vector <MyEdge> Edges;

Field Map(0, 0, 640, 480, 50, 50);
Vector2i startingMousePos(0, 0);
Vector2i startingOffset(0, 0);
int TabSelectedIndex = 0;

bool isOrientated = false;
bool isWeighed = false;
int WeightOfEdge = 1;
bool isCreating = false;
bool isConstructGraph = false;
bool isVertexDegree = false;
bool isRadiusAndDiameter = false;

//************************************************//

void onTabSelected(tgui::Gui& gui, String selectedTab)
{
	if (selectedTab == L"Додати вершину")
	{  
		TabSelectedIndex = 1;
	}
	else if (selectedTab == L"Перемістити вершину")
	{
		TabSelectedIndex = 2;
	}
	else if (selectedTab == L"З'єднати вершини")
	{
		TabSelectedIndex = 3;
	}
	else if (selectedTab == L"Видалити вершину")
	{
		TabSelectedIndex = 4;
	}
	else if (selectedTab == L"Видалити ребро")
	{
		TabSelectedIndex = 5;
	}
}
//

//************************************************//
int name1, name2;
void Dijkstra_Algorithm()//вроді вже виправив//дейкстра працює справно, але при видаленні деяких ребер щось збивається, напевно все через індексаціє не по імені при самому знаходженні шляху, тому що матриці при видаленні ребер та вершин будується правильно
{
	tgui::ChildWindow::Ptr child = theme->load("ChildWindow");
	child->setSize(300, 300);
	child->setPosition(900, 200);
	child->setTitle(L"Алгоритм Дейкстри");
	child->setTitleButtons(tgui::ChildWindow::TitleButtons::Close);
	child->connect("closed", [=]() 
	{
		child->destroy();
		Way_Dijkstra_Algorithm.clear();
	});
	gui.add(child);

	tgui::Label::Ptr label = theme->load("Label");
	label->setPosition(20, 20);
	label->setText(L"Введіть ім'я початкової вершини");
	label->setTextSize(14);
	child->add(label);

	tgui::TextBox::Ptr textBox1 = theme->load("TextBox");
	textBox1->setSize(260, 20);
	textBox1->setTextSize(14);
	textBox1->setText("1");
	textBox1->setPosition(20, 50);
	child->add(textBox1);

	label = theme->load("Label");
	label->setPosition(20, 80);
	label->setText(L"Введіть ім'я кінцевої вершини");
	label->setTextSize(14);
	child->add(label);

	tgui::TextBox::Ptr textBox2 = theme->load("TextBox");
	textBox2->setSize(260, 20);
	textBox2->setTextSize(14);
	textBox2->setText("1");
	textBox2->setPosition(20, 110);
	child->add(textBox2);

	tgui::TextBox::Ptr textBox3 = theme->load("TextBox");
	textBox3->setSize(260, 100);
	textBox3->setTextSize(14);
	textBox3->setReadOnly();
	textBox3->setScrollbar(theme->load("scrollbar"));
	textBox3->setPosition(20, 180);
	child->add(textBox3);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(20, 140);
	button->setText(L"Обрахувати");
	button->setTextSize(14);
	button->setSize(260, 20);
	button->connect("pressed", [=]() 
	{
		textBox3->setText("");
		std::string str = textBox1->getText();
		std::stringstream convertor;
		convertor << str;
		convertor >> name1;
		if (convertor.fail())
			name1 = 1;
		if (WeightOfEdge < 1)
			name1 = 1;

		std::string str2 = textBox2->getText();
		std::stringstream convertor2;
		convertor2 << str2;
		convertor2 >> name2;
		if (convertor2.fail())
			name2 = 1;
		if (WeightOfEdge < 1)
			name2 = 1;

		int start;
		int finish;

		for (int i = 0; i < Vertexs.size(); i++)
		{
			if (name1 == Vertexs.at(i).name_int)
			{
				start = i;
				break;
			}
			else
			{
				start = 0;
			}
		}

		for (int i = 0; i < Vertexs.size(); i++)
		{
			if (name2 == Vertexs.at(i).name_int)
			{
				finish = i;
				break;
			}
			else
			{
				finish = Vertexs.size() - 1;
			}
		}

		int nameMatrix[100];
		int AdjacencyMatrix[100][100];
		for (int i = 0; i < Vertexs.size(); i++)
		{
			for (int e = 0; e < Vertexs.size(); e++)
			{
				AdjacencyMatrix[i][e] = 0;
			}
		}
		//створення матриці
		for (int i = 0; i < Vertexs.size(); i++)
		{
			for (int e = 0; e < Vertexs.size(); e++)
			{
				for (int j = 0; j < Vertexs.at(i).VertexInConnectionTo.size(); j++)
				{
					if (Vertexs.at(e).name_int == Vertexs.at(i).VertexInConnectionTo.at(j))
					{
						for (int q = 0; q < Edges.size(); q++)
						{
							if (!isOrientated)
							{
								if ((Edges.at(q).VertexInConnectionFrom == Vertexs.at(i).name_int && Edges.at(q).VertexInConnectionTo == Vertexs.at(e).name_int) ||
									(Edges.at(q).VertexInConnectionTo == Vertexs.at(i).name_int && Edges.at(q).VertexInConnectionFrom == Vertexs.at(e).name_int))
								{
									AdjacencyMatrix[i][e] = Edges.at(q).WeightOfEdge;
									break;
								}
							}
							else
							{
								if (Edges.at(q).VertexInConnectionFrom == Vertexs.at(i).name_int && Edges.at(q).VertexInConnectionTo == Vertexs.at(e).name_int)
								{
									AdjacencyMatrix[i][e] = Edges.at(q).WeightOfEdge;
									break;
								}
							}
						}
						break;
					}
					else
					{
						AdjacencyMatrix[i][e] = 0;
					}
				}
			}
			nameMatrix[i] = Vertexs.at(i).name_int;
		}

		textBox1->setText(std::to_string(nameMatrix[start]));
		textBox2->setText(std::to_string(nameMatrix[finish]));

		if (start == finish)
		{
			textBox3->addText(L"Немає шляху від " + std::to_string(nameMatrix[start]) + L" до " + std::to_string(nameMatrix[finish]) + "\n");
			return;
		}

		int minindex, min, temp;
		bool PassedVertex[100];
		int DistanceToVertex[100];
		for (int i = 0; i < Vertexs.size(); i++)
		{
			PassedVertex[i] = false;
			DistanceToVertex[i] = 1000000;
		}
		DistanceToVertex[start] = 0;
		do
		{
			minindex = 1000000;
			min = 1000000;
			for (int i = 0; i < Vertexs.size(); i++)
			{
				if (!PassedVertex[i] && DistanceToVertex[i] < min)
				{
					min = DistanceToVertex[i];
					minindex = i;
				}
			}
			if (minindex != 1000000)
			{
				for (int i = 0; i < Vertexs.size(); i++)
				{
					if (AdjacencyMatrix[minindex][i] > 0)
					{
						temp = min + AdjacencyMatrix[minindex][i];
						if (temp < DistanceToVertex[i])
						{
							DistanceToVertex[i] = temp;
						}
					}
				}
				PassedVertex[minindex] = true;
			}
		} while (minindex < 1000000);

		for (int i = 0; i < Way_Dijkstra_Algorithm.size(); i++)
		{
			Way_Dijkstra_Algorithm.clear();
		}
		int tempfinish = finish;
		if (DistanceToVertex[finish] != 1000000)
		{
			int PassVertex[100];
			PassVertex[0] = nameMatrix[finish];
			int k = 1;
			int weight = DistanceToVertex[finish];
			while (finish != start)
			{
				for (int i = 0; i < Vertexs.size(); i++)
				{
					if (AdjacencyMatrix[i][finish] > 0)
					{
						int temp = weight - AdjacencyMatrix[i][finish];
						if (temp == DistanceToVertex[i])
						{
							weight = temp;
							finish = i;
							PassVertex[k] = nameMatrix[i];
							k++;
						}
					}
				}
			}

			textBox3->addText(L"Найкоротший шлях від " + std::to_string(nameMatrix[start]) + L" до " + std::to_string(nameMatrix[tempfinish]) + "\n");
			for (int i = k - 1; i >= 0; i--)
			{
				Way_Dijkstra_Algorithm.push_back(PassVertex[i]);
			}

			for (int i = 0; i < Way_Dijkstra_Algorithm.size(); i++)
			{
				textBox3->addText(std::to_string(Way_Dijkstra_Algorithm.at(i)));
				if (i != Way_Dijkstra_Algorithm.size() - 1)
					textBox3->addText(" -> ");
				else 
					textBox3->addText("\n");
			}
			textBox3->addText(L"Відстань цього шляху = " + std::to_string(DistanceToVertex[tempfinish]) + "\n");
		}
		else
		{
			textBox3->addText(L"Немає шляху від " + std::to_string(nameMatrix[start]) + L" до " + std::to_string(nameMatrix[tempfinish]) + "\n");
		}
	});
	child->add(button);
}
//

//************************************************//
void VertexsDegree()
{
	tgui::ChildWindow::Ptr child = theme->load("ChildWindow");
	child->setSize(300, 300);
	child->setPosition(800, 200);
	child->setTitle(L"Степені вершин");
	child->setTitleButtons(tgui::ChildWindow::TitleButtons::Close);
	child->connect("closed", [=]() {
		child->destroy();
		Vertexs_Degree.clear();
		isVertexDegree = false;
	});
	gui.add(child);

	tgui::TextBox::Ptr textBox1 = theme->load("TextBox");
	textBox1->setSize(260, 220);
	textBox1->setTextSize(14);
	textBox1->setReadOnly();
	textBox1->setScrollbar(theme->load("scrollbar"));
	textBox1->setPosition(20, 20);
	child->add(textBox1);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(20, 260);
	button->setText(L"Обрахувати");
	button->setTextSize(14);
	button->setSize(260, 20);
	button->connect("pressed", [=]()
	{
		Vertexs_Degree.clear();
		textBox1->setText("");
		textBox1->setText(L"Степені вершин: \n");
		for (int i = 0; i < Vertexs.size(); i++)
		{
			Vertexs_Degree.push_back(Vertexs.at(i).VertexInConnectionTo.size());
			for (int e = 0; e < Vertexs.at(i).VertexInConnectionTo.size(); e++)
			{
				if (Vertexs.at(i).name_int == Vertexs.at(i).VertexInConnectionTo.at(e))
				{
					Vertexs_Degree.at(i)++;
				}
			}
			textBox1->addText(std::to_string(Vertexs.at(i).name_int) + " = " + std::to_string(Vertexs_Degree.at(i)) + "\n");
		}
		isVertexDegree = true;
		int max = 0;
		for (int i = 0; i < Vertexs_Degree.size(); i++)
		{
			if (Vertexs_Degree.at(i) >= max)
				max = Vertexs_Degree.at(i);
		}
		textBox1->addText(L"Найбільший степінь = " + std::to_string(max) + "\n");
	});
	child->add(button);
}
//

int min(int a, int b)
{
	return (a > b ? b : a);
}

int max(int a, int b)
{
	return (a > b ? a : b);
}

//************************************************//
void RadiusAndDiameter()
{
	tgui::ChildWindow::Ptr child = theme->load("ChildWindow");
	child->setSize(300, 300);
	child->setPosition(700, 200);
	child->setTitle(L"Радіус і діаметр");
	child->setTitleButtons(tgui::ChildWindow::TitleButtons::Close);
	child->connect("closed", [=]() {
		child->destroy();
		CentralOrPeripheral.clear();
		isRadiusAndDiameter = false;
	});
	gui.add(child);

	tgui::TextBox::Ptr textBox1 = theme->load("TextBox");
	textBox1->setSize(260, 220);
	textBox1->setTextSize(14);
	textBox1->setReadOnly();
	textBox1->setScrollbar(theme->load("scrollbar"));
	textBox1->setPosition(20, 20);
	child->add(textBox1);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(20, 260);
	button->setText(L"Обрахувати");
	button->setTextSize(14);
	button->setSize(260, 20);
	button->connect("pressed", [=]()
	{
		isRadiusAndDiameter = true;
		CentralOrPeripheral.clear();
		textBox1->setText("");
		
		int DistanceMatrix[100][100];
		for (int i = 0; i < Vertexs.size(); i++)
		{
			for (int e = 0; e < Vertexs.size(); e++)
			{
				DistanceMatrix[i][e] = 1000000;
			}
		}

		for (int i = 0; i < Vertexs.size(); i++)
		{
			for (int e = 0; e < Vertexs.size(); e++)
			{
				for (int j = 0; j < Vertexs.at(i).VertexInConnectionTo.size(); j++)
				{
					if (Vertexs.at(e).name_int == Vertexs.at(i).VertexInConnectionTo.at(j))
					{
						for (int q = 0; q < Edges.size(); q++)
						{
							if (!isOrientated)
							{
								if ((Edges.at(q).VertexInConnectionFrom == Vertexs.at(i).name_int && Edges.at(q).VertexInConnectionTo == Vertexs.at(e).name_int) ||
									(Edges.at(q).VertexInConnectionTo == Vertexs.at(i).name_int && Edges.at(q).VertexInConnectionFrom == Vertexs.at(e).name_int))
								{
									DistanceMatrix[i][e] = Edges.at(q).WeightOfEdge;
									break;
								}
							}
							else
							{
								if (Edges.at(q).VertexInConnectionFrom == Vertexs.at(i).name_int && Edges.at(q).VertexInConnectionTo == Vertexs.at(e).name_int)
								{
									DistanceMatrix[i][e] = Edges.at(q).WeightOfEdge;
									break;
								}
							}
						}
						break;
					}
					else
					{
						DistanceMatrix[i][e] = 1000000;
					}
				}
			}
		}

		int Radius = 1000000;
		int Diameter = 0;
		
		for (int k = 0; k < Vertexs.size(); k++) 
		{
			for (int i = 0; i < Vertexs.size(); i++) 
			{
				for (int j = 0; j < Vertexs.size(); j++)
				{
					DistanceMatrix[i][j] = min(DistanceMatrix[i][j], DistanceMatrix[i][k] + DistanceMatrix[k][j]);
				}
			}
		}


		bool isCoherent = true;
		for (int i = 0; i < Vertexs.size(); i++)
		{
			for (int j = 0; j < Vertexs.size(); j++)
			{
				if (i != j)
				{
					if (DistanceMatrix[i][j] == 1000000)
					{
						isCoherent = false;
					}
				}
				else
				{
					DistanceMatrix[i][j] = 0;
				}
			}
		}


		std::vector<int> Eccentricities;
		for (int i = 0; i < Vertexs.size(); i++)
		{
			Eccentricities.push_back(0);
		}

		for (int i = 0; i < Vertexs.size(); i++)
		{
			CentralOrPeripheral.push_back(0);
		}

		if (isCoherent)
		{
			for (int i = 0; i < Vertexs.size(); i++)
			{
				for (int j = 0; j < Vertexs.size(); j++)
				{
					if (i != j)
					{
						Eccentricities.at(i) = max(Eccentricities.at(i), DistanceMatrix[i][j]);
					}
				}
			}

			textBox1->addText(L"Ексцентриситети:\n");
			for (int i = 0; i < Eccentricities.size(); i++)
			{
				Radius = min(Eccentricities.at(i), Radius);
				Diameter = max(Eccentricities.at(i), Diameter);
				textBox1->addText(std::to_string(Vertexs.at(i).name_int) + " = " + std::to_string(Eccentricities.at(i)) + "\n");
			}
			textBox1->addText(L"Діаметр = " + std::to_string(Diameter) + "\n" + L"Радіус = " + std::to_string(Radius) + "\n");

			
			for (int i = 0; i < Vertexs.size(); i++)
			{
				if (Radius == Eccentricities.at(i))
				{
					CentralOrPeripheral.at(i) = 1;
				}
				else if (Diameter == Eccentricities.at(i))
				{
					CentralOrPeripheral.at(i) = 2;
				}
			}
			textBox1->addText(L"Центральні вершини графа:\n");
			for (int i = 0; i < Vertexs.size(); i++)
			{
				if(CentralOrPeripheral.at(i) == 1)
				{ 
					textBox1->addText(std::to_string(Vertexs.at(i).name_int) + "; ");
				}
			}
			textBox1->addText(L"\nПериферійні вершини графа:\n");
			for (int i = 0; i < Vertexs.size(); i++)
			{
				if (CentralOrPeripheral.at(i) == 2)
				{
					textBox1->addText(std::to_string(Vertexs.at(i).name_int) + "; ");
				}
			}

		}
		else
		{
			textBox1->addText(L"Даний граф не є зв'язним.");
		}
		
		
	});
	child->add(button);
}
//

//************************************************//
//відобразити матрицю суміжності:
void ShowAdjacencyMatrix()
{
	RenderWindow windowAdjacencyMatrix(VideoMode(700, 500), L"Матриця суміжності", Style::Titlebar | Style::Close);
	tgui::Gui guiAnjacencyMatrix{ windowAdjacencyMatrix };
	TabSelectedIndex = 0;
	tabs->deselect();

	tgui::TextBox::Ptr textBox = theme->load("TextBox");
	textBox->setSize(700, 500);
	textBox->setScrollbar(theme->load("scrollbar"));
	textBox->setTextSize(14);
	textBox->setPosition(0, 0);
	textBox->setReadOnly();
	textBox->addText(L"В|В\t");

	for (int i = 0; i < Vertexs.size(); i++)
	{
		textBox->addText(std::to_string(Vertexs.at(i).name_int));
		textBox->addText("\t");
	}
	textBox->addText("\n");

	std::cout << "hello\n";
	for (int i = 0; i < Vertexs.size(); i++)
	{
		bool inConnection = false;
		textBox->addText(std::to_string(Vertexs.at(i).name_int));
		textBox->addText("\t\t");
		for (int e = 0; e < Vertexs.size(); e++)
		{
			for (int j = 0; j < Vertexs.at(i).VertexInConnectionTo.size(); j++)
			{
				if (Vertexs.at(e).name_int == Vertexs.at(i).VertexInConnectionTo.at(j))
				{
					inConnection = true;
					break;
				}

			}
			if (inConnection)
			{
				textBox->addText("1");
				textBox->addText("\t");
				inConnection = false;
			}
			else
			{
				textBox->addText("0");
				textBox->addText("\t");
			}
		}
		textBox->addText("\n");
	}
	guiAnjacencyMatrix.add(textBox);

	while (windowAdjacencyMatrix.isOpen())
	{
		Event event;
		while (windowAdjacencyMatrix.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				windowAdjacencyMatrix.close();
				guiAnjacencyMatrix.setWindow(window);
			}
			guiAnjacencyMatrix.handleEvent(event);
		}
		windowAdjacencyMatrix.clear(Color(120, 120, 120));
		guiAnjacencyMatrix.draw();
		windowAdjacencyMatrix.display();
	}
}
//

//************************************************//
//відобразити матрицю інцидентності:
void ShowIncidenceMatrix()
{
	RenderWindow windowIncidanceMatrix(VideoMode(700, 500), L"Матриця інцидентності", Style::Titlebar | Style::Close);
	tgui::Gui guiIncidenceMatrix{ windowIncidanceMatrix };
	TabSelectedIndex = 0;
	tabs->deselect();

	tgui::TextBox::Ptr textBox = theme->load("TextBox");
	textBox->setSize(700, 500);
	textBox->setScrollbar(theme->load("scrollbar"));
	textBox->setTextSize(14);
	textBox->setPosition(0, 0);
	textBox->setReadOnly();
	textBox->addText(L"Г|В \t");

	for (int i = 0; i < Vertexs.size(); i++)
	{
		textBox->addText(std::to_string(Vertexs.at(i).name_int));
		textBox->addText(" \t");
	}
	textBox->addText("\n");

	for (int i = 0; i < Edges.size(); i++)
	{
		textBox->addText(std::to_string(Edges.at(i).name_int));
		textBox->addText("\t\t");
		for (int e = 0; e < Vertexs.size(); e++)
		{
			if (!isOrientated)
			{
				if (Edges.at(i).VertexInConnectionFrom == Vertexs.at(e).name_int || Edges.at(i).VertexInConnectionTo == Vertexs.at(e).name_int)
				{
					textBox->addText(" 1");
					textBox->addText("\t");
				}
				else
				{
					textBox->addText(" 0");
					textBox->addText("\t");
				}
			}
			else
			{
				if (Edges.at(i).VertexInConnectionFrom == Vertexs.at(e).name_int && Edges.at(i).VertexInConnectionTo == Vertexs.at(e).name_int)
				{
					textBox->addText(" @");
					textBox->addText("\t");
				}
				else if (Edges.at(i).VertexInConnectionTo == Vertexs.at(e).name_int)
				{
					textBox->addText("-1");
					textBox->addText("\t");
				}
				else if(Edges.at(i).VertexInConnectionFrom == Vertexs.at(e).name_int)
				{
					textBox->addText(" 1");
					textBox->addText("\t");
				}
				else
				{
					textBox->addText(" 0");
					textBox->addText("\t");
				}
			}
		}
		
		textBox->addText("\n");
	}
	guiIncidenceMatrix.add(textBox);

	while (windowIncidanceMatrix.isOpen())
	{
		Event event;
		while (windowIncidanceMatrix.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				windowIncidanceMatrix.close();
				guiIncidenceMatrix.setWindow(window);
			}
			guiIncidenceMatrix.handleEvent(event);
		}
		windowIncidanceMatrix.clear(Color(120, 120, 120));
		guiIncidenceMatrix.draw();
		windowIncidanceMatrix.display();
	}
}
//

//************************************************//

void CreateEdgeDialog(int i, std::string strE)
{
	isCreating = true;
	tgui::ChildWindow::Ptr child = theme->load("ChildWindow");
	child->setSize(180, 120);
	child->setPosition(Mouse::getPosition(window).x - 90, Mouse::getPosition(window).y - 90);
	child->setTitle(L"Нове ребро");
	child->setTitleButtons(tgui::ChildWindow::TitleButtons::Close);
	child->connect("closed", [=]()
	{
		WeightOfEdge = 1;
		isCreating = false;
		child->destroy();
	});
	gui.add(child);

	tgui::Label::Ptr label = theme->load("Label");
	label->setPosition(20, 20);
	label->setText(L"Введіть вагу ребра:");
	label->setTextSize(14);
	child->add(label);

	tgui::TextBox::Ptr textBox = theme->load("TextBox");
	textBox->setSize(140, 20);
	textBox->setTextSize(14);
	textBox->setText("1");
	textBox->setPosition(20, 50);
	child->add(textBox);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(20, 80);
	button->setText(L"Створити");
	button->setTextSize(14);
	button->setSize(140, 20);
	button->connect("pressed", [=]()
	{
		std::string str = textBox->getText();
		std::stringstream convertor;
		convertor << str;
		convertor >> WeightOfEdge;
		if (convertor.fail())
			WeightOfEdge = 1;
		if (WeightOfEdge < 1 || WeightOfEdge > 1000)
			WeightOfEdge = 1;
		std::cout << str << " = " << WeightOfEdge << "\n";
		child->destroy();

		if (SelectedVertex != i)
		{
			if (isOrientated)
			{
				Vertexs.at(SelectedVertex).AddConectionTo(Vertexs.at(i).name_int);
				Vertexs.at(i).AddConectionFrom(Vertexs.at(SelectedVertex).name_int);
			}
			else
			{
				Vertexs.at(SelectedVertex).AddConectionTo(Vertexs.at(i).name_int);
				Vertexs.at(i).AddConectionFrom(Vertexs.at(SelectedVertex).name_int);
				Vertexs.at(SelectedVertex).AddConectionFrom(Vertexs.at(i).name_int);
				Vertexs.at(i).AddConectionTo(Vertexs.at(SelectedVertex).name_int);
			}
			Edges.push_back(MyEdge(Vertexs.at(SelectedVertex), Vertexs.at(i), strE, &edgeFont, NameOfEdge, WeightOfEdge));
			NameOfEdge++;
			counterE++;
		}
		else
		{
			Vertexs.at(SelectedVertex).AddConectionTo(Vertexs.at(i).name_int);
			Vertexs.at(SelectedVertex).AddConectionFrom(Vertexs.at(i).name_int);
			Edges.push_back(MyEdge(Vertexs.at(i), Vertexs.at(i), strE, &edgeFont, NameOfEdge, 2));
			NameOfEdge++;
			counterE++;
		}	

		isCreating = false; });
	child->add(button);
}
//

void CreateGraphDialog()
{
	tgui::Panel::Ptr panel1 = theme->load("Panel");
	panel1->setSize(350, 200);
	panel1->setPosition(Width / 2 - 150, Height / 2 - 150);
	gui.add(panel1);

	tgui::RadioButton::Ptr radioButton1 = theme->load("RadioButton");
	radioButton1->setPosition(20, 50);
	radioButton1->setText(L"Орієнтований");
	radioButton1->setSize(20, 20);
	radioButton1->connect("checked", [=]() { isOrientated = true; });
	panel1->add(radioButton1);

	radioButton1 = theme->load("RadioButton");
	radioButton1->setPosition(20, 90);
	radioButton1->setText(L"Не орієнтований");
	radioButton1->connect("checked", [=]() { isOrientated = false; });
	radioButton1->setSize(20, 20);
	panel1->add(radioButton1);
	radioButton1->check();

	tgui::RadioButton::Ptr radioButton2 = theme->load("RadioButton");
	radioButton2->setPosition(Width / 2 - 150 + 200, Height / 2 - 150 + 50);
	radioButton2->setText(L"Зважений");
	radioButton2->setSize(20, 20);
	radioButton2->connect("checked", [=]() { isWeighed = true;});
	gui.add(radioButton2);

	tgui::RadioButton::Ptr radioButton3 = theme->load("RadioButton");
	radioButton3 = theme->load("RadioButton");
	radioButton3->setPosition(Width / 2 - 150 + 200, Height / 2 - 150 + 90);
	radioButton3->setText(L"Не зважений");
	radioButton3->connect("checked", [=]() { isWeighed = false;});
	radioButton3->setSize(20, 20);
	gui.add(radioButton3);
	radioButton3->check();

	tgui::Label::Ptr label = theme->load("Label");
	label->setPosition(Width / 2 - 150 + 110, Height / 2 - 150 + 20);
	label->setText(L"Оберіть тип графу:");
	label->setTextSize(14);
	gui.add(label);

	tgui::Button::Ptr button = theme->load("Button");
	button->setPosition(Width / 2 - 150 + 75, Height / 2 - 150 + 140);
	button->setText(L"Розпочати роботу");
	button->setTextSize(14);
	button->setSize(200, 30);
	button->connect("pressed", [=]() 
	{ 
		isConstructGraph = true; 
		gui.remove(panel1);
		gui.remove(radioButton2);
		gui.remove(radioButton3);
		gui.remove(label);
		gui.remove(button);
		std::cout << "isOrientated = " << isOrientated << "\n";
		std::cout << "isWeighed = " << isWeighed << "\n";
	});
	gui.add(button);
}

//************************************************//

void Draw_Map()
{
	line.setSize(Vector2f(1, 900));
	line.setFillColor(Color(150, 0, 0));
	for (int i = 0; i <= 32; i++)
	{
		line.setPosition(Vector2f(i * 50, 100));
		window.draw(line);
	}

	line.setSize(Vector2f(1600, 1));
	for (int i = 2; i <= 18; i++)
	{
		line.setPosition(Vector2f(0, i * 50));
		window.draw(line);
	}
}


//************************************************//

bool isCross(int x, int y, int s)
{
	if (Mouse::getPosition(window).x / s == x &&
		Mouse::getPosition(window).y / s == y)
		return true;
	else
		return false;
}

double Distanse(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

bool isCrossEdgeLoop(MyEdge edge)
{
	if (Mouse::getPosition(window).x >= edge.x1 * 50 - 10 &&
		Mouse::getPosition(window).y >= edge.y1 * 50 - 10 &&
		Mouse::getPosition(window).x <= edge.x1 * 50 - 10 + 24 &&
		Mouse::getPosition(window).y <= edge.y1 * 50 - 10 + 24)
		return true;
	else
		return false;
}

bool isCrossEdge(MyEdge edge)
{
	double x_center;
	double y_center;
	int mousePosX = Mouse::getPosition(window).x;
	int mousePosY = Mouse::getPosition(window).y;

	if (edge.x1 > edge.x2)
		x_center = edge.x2 * 50 + 25 + ((edge.x1 * 50 + 25 - edge.x2 * 50 + 25) / 2);
	else if (edge.x1 < edge.x2)
		x_center = edge.x1 * 50 + 25 + ((edge.x2 * 50 + 25 - edge.x1 * 50 + 25) / 2);
	else x_center = edge.x1 * 50 + 25;

	if (edge.y1 > edge.y2)
		y_center = edge.y2 * 50 + 25 + ((edge.y1 * 50 + 25 - edge.y2 * 50 + 25) / 2);
	else if (edge.y1 < edge.y2)
		y_center = edge.y1 * 50 + 25 + ((edge.y2 * 50 + 25 - edge.y1 * 50 + 25) / 2);
	else y_center = edge.y1 * 50 + 25;

	double dis_to_center = Distanse((int)x_center, (int)y_center, mousePosX, mousePosY);
	double dis1 = Distanse(edge.x1 * 50 + 25, edge.y1 * 50 + 25, edge.x2 * 50 + 25, edge.y2 * 50 + 25);
	double dis2 = Distanse(mousePosX, mousePosY, edge.x2 * 50 + 25, edge.y2 * 50 + 25);
	double dis3 = Distanse(edge.x1 * 50 + 25, edge.y1 * 50 + 25, mousePosX, mousePosY);
	double P_2 = (dis1 + dis2 + dis3) / 2;
	double h = 2 * sqrt(P_2 * (P_2 - dis1) * (P_2 - dis2) * (P_2 - dis3)) / dis1;
	if (h <= 15 && dis_to_center <= (dis1 / 2))
		return true;
	else
		return false;
}

//************************************************//

void update(Event event)
{
	std::string strV = std::to_string(NameOfVertex);
	std::string strE = std::to_string(NameOfEdge);

	if (counter == 0)
		NameOfVertex = 1;
	if (counterE == 0)
		NameOfEdge = 1;

	bool Select = false;
	bool isCr = false;
	if (!isCreating && isConstructGraph)
	{
		switch (TabSelectedIndex)
		{
		case 1:
			// додавання вершин:
			if (event.type == event.MouseButtonReleased && Mouse::getPosition(window).y > 100)
			{
				if (event.key.code == Mouse::Button::Left)
				{
					for (int i = 0; i < Vertexs.size(); i++)
					{
						if (isCross(Vertexs.at(i).x, Vertexs.at(i).y, 50))
						{
							isCr = true;
						}
					}
					if (!isCr)
					{
						std::cout << NameOfVertex << "\n";
						Vertexs.push_back(MyVertex(Mouse::getPosition(window).x / 50, Mouse::getPosition(window).y / 50, strV, &font, NameOfVertex));
						counter++;
						NameOfVertex++;
						std::cout << counter << "\n";
						isCr = false;
					}
				}
			}
			//
			break;
		case 2:
			//переміщення вершини
			if (Mouse::isButtonPressed(Mouse::Button::Left) && Mouse::getPosition(window).y > 100 && Vertexs.size() > 0)
			{
				if (!Select)
				{
					for (int i = 0; i < Vertexs.size(); i++)
					{
						if (isCross(Vertexs.at(i).x, Vertexs.at(i).y, 50) && i != SelectedVertex)
						{
							SelectedVertex = i;
							break;
						}
					}
				}
				Vertexs.at(SelectedVertex).x = Mouse::getPosition(window).x / 50;
				Vertexs.at(SelectedVertex).y = Mouse::getPosition(window).y / 50;
				for (int i = 0; i < Edges.size(); i++)
				{
					if (Edges.at(i).VertexInConnectionFrom == Vertexs.at(SelectedVertex).name_int)
					{
						Edges.at(i).x1 = Vertexs.at(SelectedVertex).x;
						Edges.at(i).y1 = Vertexs.at(SelectedVertex).y;
					}
					else if (Edges.at(i).VertexInConnectionTo == Vertexs.at(SelectedVertex).name_int)
					{
						Edges.at(i).x2 = Vertexs.at(SelectedVertex).x;
						Edges.at(i).y2 = Vertexs.at(SelectedVertex).y;
					}
				}
			}
			else Select = false;
			//
			break;
		case 3:
			//з'єднання верщин ребром:
			if (event.type == event.MouseButtonReleased  && Mouse::getPosition(window).y > 100)
			{
				if (event.key.code == Mouse::Button::Left)
				{
					bool isConnection = false;
					for (int i = 0; i < Vertexs.size(); i++)
					{
						if (isCross(Vertexs.at(i).x, Vertexs.at(i).y, 50))
						{
							for (int e = 0; e < Vertexs.at(SelectedVertex).VertexInConnectionTo.size(); e++)
							{
								if (Vertexs.at(SelectedVertex).VertexInConnectionTo.at(e) == Vertexs.at(i).name_int)
								{
									isConnection = true;
									break;
								}
							}
							if(!isOrientated)
							{
								for (int e = 0; e < Vertexs.at(SelectedVertex).VertexInConnectionFrom.size(); e++)
								{
									if (Vertexs.at(SelectedVertex).VertexInConnectionFrom.at(e) == Vertexs.at(i).name_int)
									{
										isConnection = true;
										break;
									}
								}
							}
							if (!isConnection)
							{
								if(isWeighed)
								{
									CreateEdgeDialog(i, strE);
									break;
								}
								else
								{
									if (SelectedVertex != i)
									{
										if (isOrientated)
										{
											Vertexs.at(SelectedVertex).AddConectionTo(Vertexs.at(i).name_int);
											Vertexs.at(i).AddConectionFrom(Vertexs.at(SelectedVertex).name_int);
										}
										else
										{ 
											Vertexs.at(SelectedVertex).AddConectionTo(Vertexs.at(i).name_int);
											Vertexs.at(i).AddConectionFrom(Vertexs.at(SelectedVertex).name_int);
											Vertexs.at(SelectedVertex).AddConectionFrom(Vertexs.at(i).name_int);
											Vertexs.at(i).AddConectionTo(Vertexs.at(SelectedVertex).name_int);
										}
										Edges.push_back(MyEdge(Vertexs.at(SelectedVertex), Vertexs.at(i), strE, &edgeFont, NameOfEdge, WeightOfEdge));
										NameOfEdge++;
										counterE++;
									}
									else
									{
										Vertexs.at(SelectedVertex).AddConectionTo(Vertexs.at(i).name_int);
										Vertexs.at(SelectedVertex).AddConectionFrom(Vertexs.at(i).name_int);
										Edges.push_back(MyEdge(Vertexs.at(i), Vertexs.at(i), strE, &edgeFont, NameOfEdge, 2));
										NameOfEdge++;
										counterE++;
									}
								}
							}
						}
					}
				}
			}
			//
			break;
		case 4:
			//видалення вершини:
			if (event.type == event.MouseButtonReleased  && Mouse::getPosition(window).y > 100)
			{
				if (event.key.code == Mouse::Button::Left)
				{
					for (int i = 0; i < Vertexs.size(); i++)
					{
						if (isCross(Vertexs.at(i).x, Vertexs.at(i).y, 50))
						{
							for (int e = 0; e < Vertexs.at(i).VertexInConnectionTo.size(); e++)
							{
								for (int j = 0; j < Vertexs.size(); j++)
								{
									if (Vertexs.at(i).VertexInConnectionTo.at(e) == Vertexs.at(j).name_int)
									{
										Vertexs.at(j).RemoveConnectionFrom(Vertexs.at(i).name_int);
										break;
									}
								}
							}
							for (int e = 0; e < Vertexs.at(i).VertexInConnectionFrom.size(); e++)
							{
								for (int j = 0; j < Vertexs.size(); j++)
								{
									if (Vertexs.at(i).VertexInConnectionFrom.at(e) == Vertexs.at(j).name_int)
									{
										Vertexs.at(j).RemoveConnectionTo(Vertexs.at(i).name_int);
										break;
									}
								}
							}
							for (int e = 0; e < Edges.size(); e++)
							{
								if (Vertexs.at(i).name_int == Edges.at(e).VertexInConnectionFrom)
								{
									Edges.erase(Edges.begin() + e);
									counterE--;
									e = 0;
									std::cout << "-\n";
								}
								else if (Vertexs.at(i).name_int == Edges.at(e).VertexInConnectionTo)
								{
									Edges.erase(Edges.begin() + e);
									counterE--;
									e = 0;
									std::cout << "-\n";
								}
							}
							// додаткова перевірка
							for (int e = 0; e < Edges.size(); e++)
							{
								if (Vertexs.at(i).name_int == Edges.at(e).VertexInConnectionFrom)
								{
									Edges.erase(Edges.begin() + e);
									counterE--;
									e = 0;
									std::cout << "-\n";
								}
								else if (Vertexs.at(i).name_int == Edges.at(e).VertexInConnectionTo)
								{
									Edges.erase(Edges.begin() + e);
									counterE--;
									e = 0;
									std::cout << "-\n";
								}
							}
							SelectedVertex = 0;
							Vertexs.erase(Vertexs.begin() + i);
							counter--;
							std::cout << counter << "\n";
							break;
						}
					}
				}
			}
			//
			break;
		case 5:
			// видалення ребер
			if (event.type == event.MouseButtonReleased  && Mouse::getPosition(window).y > 100)
			{
				if (event.key.code == Mouse::Button::Left)
				{
					for (int i = 0; i < Edges.size(); i++)
					{
						if (isCrossEdgeLoop(Edges.at(i)))
						{
							int name1 = Edges.at(i).VertexInConnectionFrom;
							int name2 = Edges.at(i).VertexInConnectionTo;
							int index1, index2;
							int helper = 0;
							for (int e = 0; e < Vertexs.size(); e++)
							{
								if (name1 == Vertexs.at(e).name_int)
								{
									index1 = e;
									helper++;
								}
								if (name2 == Vertexs.at(e).name_int)
								{
									index2 = e;
									helper++;
								}
								if (helper == 2)
									break;
							}
							if (name1 == name2)
							{
								for (int e = 0; e < Vertexs.at(index1).VertexInConnectionTo.size(); e++)
								{
									if (Vertexs.at(index1).name_int == Vertexs.at(index1).VertexInConnectionTo.at(e))
									{
										Vertexs.at(index1).RemoveConnectionTo(Vertexs.at(index1).name_int);
										Vertexs.at(index1).RemoveConnectionFrom(Vertexs.at(index1).name_int);
										break;
									}
								}
								Edges.erase(Edges.begin() + i);
								counterE--;
								break;
							}
						}
						else if (isCrossEdge(Edges.at(i)))
						{
							int name1 = Edges.at(i).VertexInConnectionFrom;
							int name2 = Edges.at(i).VertexInConnectionTo;
							int index1, index2;
							int helper = 0;
							for (int e = 0; e < Vertexs.size(); e++)
							{
								if (name1 == Vertexs.at(e).name_int)
								{
									index1 = e;
									helper++;
								}
								if (name2 == Vertexs.at(e).name_int)
								{
									index2 = e;
									helper++;
								}
								if (helper == 2)
									break;
							}
							for (int e = 0; e < Vertexs.at(index1).VertexInConnectionTo.size(); e++)
							{
								if (Vertexs.at(index2).name_int == Vertexs.at(index1).VertexInConnectionTo.at(e))
								{
									Vertexs.at(index1).RemoveConnectionTo(Vertexs.at(index2).name_int);
									break;
								}
							}

							for (int e = 0; e < Vertexs.at(index1).VertexInConnectionFrom.size(); e++)
							{
								if (Vertexs.at(index2).name_int == Vertexs.at(index1).VertexInConnectionFrom.at(e))
								{
									Vertexs.at(index1).RemoveConnectionFrom(Vertexs.at(index2).name_int);
									break;
								}
							}

							for (int e = 0; e < Vertexs.at(index2).VertexInConnectionTo.size(); e++)
							{
								if (Vertexs.at(index1).name_int == Vertexs.at(index2).VertexInConnectionTo.at(e))
								{
									Vertexs.at(index2).RemoveConnectionTo(Vertexs.at(index1).name_int);
									break;
								}
							}

							for (int e = 0; e < Vertexs.at(index2).VertexInConnectionFrom.size(); e++)
							{
								if (Vertexs.at(index1).name_int == Vertexs.at(index2).VertexInConnectionFrom.at(e))
								{
									Vertexs.at(index2).RemoveConnectionFrom(Vertexs.at(index1).name_int);
									break;
								}
							}
							Edges.erase(Edges.begin() + i);
							counterE--;
							break;
						}
					}
				}
			}
			//
			break;
		}
	}

	//вибір вершини:
	if (event.type == event.MouseButtonReleased)
	{
		if (event.key.code == Mouse::Button::Right)
		{
			for (int i = 0; i < Vertexs.size(); i++)
			{
				if (isCross(Vertexs.at(i).x, Vertexs.at(i).y, 50) && i != SelectedVertex)
				{
					SelectedVertex = i;
					std::cout << "-" << std::endl;
					break;
				}
			}
		}
	}
	//

}

void Drawing()
{
	Draw_Map();
	int helper2 = 0;
	//рисування ребер:
	for (int i = 0; i < Edges.size(); i++)
	{
		int x1, y1, x2, y2;
		int x_center;
		int y_center;
		int helper = 0;

		for (int e = 0; e < Vertexs.size(); e++)
		{
			if (Edges.at(i).VertexInConnectionTo == Vertexs.at(e).name_int)
			{
				x1 = Edges.at(i).x1 * 50 + 25;
				y1 = Edges.at(i).y1 * 50 + 25;
				helper++;
			}
			else if (Edges.at(i).VertexInConnectionFrom == Vertexs.at(e).name_int)
			{
				x2 = Edges.at(i).x2 * 50 + 25;
				y2 = Edges.at(i).y2 * 50 + 25;
				helper++;
			}
			if (helper == 2)
			{
				helper = 0;
				if (isOrientated)
				{
					helper2++;
				}
				break;
			}
		}

		if (Edges.at(i).VertexInConnectionTo == Edges.at(i).VertexInConnectionFrom)
		{
			edgeLoopSprite.setPosition(x1 - 30, y1 - 30);
			window.draw(edgeLoopSprite);

			edgeSprite.setPosition(x1 - 35, y1 - 35);
			window.draw(edgeSprite);
			if (Edges.at(i).name_int < 10)
				Edges.at(i).name.setPosition(Vector2f(x1 - 35 + 8, y1 - 35 + 2));
			else if (Edges.at(i).name_int < 100)
				Edges.at(i).name.setPosition(Vector2f(x1 - 35 + 2, y1 - 35 + 2));
			else
				Edges.at(i).name.setPosition(Vector2f(x1 - 35 - 2, y1 - 35 + 2));
			window.draw(Edges.at(i).name);
		}
		else
		{
			Vertex line[] =
			{
				Vertex(Vector2f(x1, y1)),
				Vertex(Vector2f(x2, y2))
			};
			bool isRed = false;
			for (int q = 0; q < Way_Dijkstra_Algorithm.size(); q++)
			{
				if (q == Way_Dijkstra_Algorithm.size() - 1)
					break;
				if (isOrientated && Edges.at(i).VertexInConnectionFrom == Way_Dijkstra_Algorithm[q] && Edges.at(i).VertexInConnectionTo == Way_Dijkstra_Algorithm[q + 1])
				{
					line[0].color = Color::Green;
					line[1].color = Color::Green;
					isRed = true;
					break;
				}
				if ((Edges.at(i).VertexInConnectionFrom == Way_Dijkstra_Algorithm[q] && Edges.at(i).VertexInConnectionTo == Way_Dijkstra_Algorithm[q + 1]) ||
					(Edges.at(i).VertexInConnectionTo == Way_Dijkstra_Algorithm[q] && Edges.at(i).VertexInConnectionFrom == Way_Dijkstra_Algorithm[q + 1]))
				{
					line[0].color = Color::Green;
					line[1].color = Color::Green;
					break;
				}
			}
			window.draw(line, 2, Lines);

			if (isOrientated)
			{
				double angel = atan2(y2 - y1, x2 - x1);
				Vertex arrow[] =
				{
					Vertex(Vector2f(cos(angel + 0.2 + 3.14) * 30 + x2, sin(angel + 0.2 + 3.14) * 30 + y2)),
					Vertex(Vector2f(x2, y2)),
					Vertex(Vector2f(cos(angel - 0.2 + 3.14) * 30 + x2, sin(angel - 0.2 + 3.14) * 30 + y2))
				};
				if (isRed)
				{
					arrow[0].color = Color::Green;
					arrow[1].color = Color::Green;
					arrow[2].color = Color::Green;
				}
				window.draw(arrow, 3, TriangleStrip);
			}


			if (x1 > x2)
				x_center = x2 + ((x1 - x2) / 2);
			else if (x1 < x2)
				x_center = x1 + ((x2 - x1) / 2);
			else x_center = x1;

			if (y1 > y2)
				y_center = y2 + ((y1 - y2) / 2);
			else if (y1 < y2)
				y_center = y1 + ((y2 - y1) / 2);
			else y_center = y1;

			if (isOrientated)
			{
				if (helper2 % 2 == 0)
				{
					y_center += 10;
				}
				else
				{
					y_center -= 10;
				}
			}

			edgeSprite.setPosition(x_center - 12, y_center - 12);
			window.draw(edgeSprite);

			if (Edges.at(i).name_int < 10)
				Edges.at(i).name.setPosition(Vector2f(x_center - 12 + 8, y_center - 12 + 2));
			else if (Edges.at(i).name_int < 100)
				Edges.at(i).name.setPosition(Vector2f(x_center - 12 + 2, y_center - 12 + 2));
			else
				Edges.at(i).name.setPosition(Vector2f(x_center - 12 - 4, y_center - 12 + 2));
			window.draw(Edges.at(i).name);

			EdgeWeight.setString("(" + std::to_string(Edges.at(i).WeightOfEdge) + ")");
			EdgeWeight.setPosition(Vector2f(x_center + 12, y_center - 12));
			window.draw(EdgeWeight);

		}
	}

	//рисування вершин:
	for (int i = 0; i < Vertexs.size(); i++)
	{
		if (i == SelectedVertex)
		{
			selectedVertexSprite.setPosition(Vertexs.at(i).x * 50 + 9, Vertexs.at(i).y * 50 + 9);
			window.draw(selectedVertexSprite);
			if (Vertexs.at(i).name_int < 10)
				Vertexs.at(i).name.setPosition(Vector2f(Vertexs.at(i).x * 50 + 20, Vertexs.at(i).y * 50 + 15));
			else if (Vertexs.at(i).name_int < 100)
				Vertexs.at(i).name.setPosition(Vector2f(Vertexs.at(i).x * 50 + 15, Vertexs.at(i).y * 50 + 15));
			else
				Vertexs.at(i).name.setPosition(Vector2f(Vertexs.at(i).x * 50 + 10, Vertexs.at(i).y * 50 + 15));
			Vertexs.at(i).name.setFillColor(Color::Blue);
			window.draw(Vertexs.at(i).name);
			Vertexs.at(i).name.setFillColor(Color::Black);
		}
		else
		{
			vertexSprite.setPosition(Vertexs.at(i).x * 50 + 9, Vertexs.at(i).y * 50 + 9);
			window.draw(vertexSprite);
			if (Vertexs.at(i).name_int < 10)
				Vertexs.at(i).name.setPosition(Vector2f(Vertexs.at(i).x * 50 + 20, Vertexs.at(i).y * 50 + 15));
			else if (Vertexs.at(i).name_int < 100)
				Vertexs.at(i).name.setPosition(Vector2f(Vertexs.at(i).x * 50 + 15, Vertexs.at(i).y * 50 + 15));
			else
				Vertexs.at(i).name.setPosition(Vector2f(Vertexs.at(i).x * 50 + 10, Vertexs.at(i).y * 50 + 15));
			window.draw(Vertexs.at(i).name);
		}
		if (isVertexDegree)
		{
			VertexDegree.setPosition(Vector2f(Vertexs.at(i).x * 50 + 30, Vertexs.at(i).y * 50 - 5));
			VertexDegree.setString(std::to_string(Vertexs_Degree.at(i)));
			window.draw(VertexDegree);
		}
		if (isRadiusAndDiameter)
		{
			if (CentralOrPeripheral.at(i) == 1)
			{
				CentOrPer.setFillColor(Color::Yellow);
				CentOrPer.setString(L"Центр.");
				CentOrPer.setPosition(Vector2f(Vertexs.at(i).x * 50, Vertexs.at(i).y * 50 - 7));
				window.draw(CentOrPer);
			}
			else if (CentralOrPeripheral.at(i) == 2)
			{
				CentOrPer.setFillColor(Color::Cyan);
				CentOrPer.setString(L"Периф.");
				CentOrPer.setPosition(Vector2f(Vertexs.at(i).x * 50, Vertexs.at(i).y * 50 - 7));
				window.draw(CentOrPer);
			}
		}
	}
	//
}
/*
0 = MainMenu
1 = GraphConstructor
2 = Informer
3 = AboutProgram
*/
int WhatModeNow = 0;
Sprite MenuSprite;


//************************************************//
void LoadMainMenu()
{
	MenuTexture.loadFromFile("res/menu_texture.png");
	MenuTexture.setRepeated(true);
	/*tgui::Picture::Ptr picture;
	picture->setTexture({ MenuTexture, {100, 100, 200, 200} });
	picture->setPosition(0, 0);
	picture->setSize(1600, 900);
	guiMainMenu.add(picture);*/

	MenuSprite.setTexture(MenuTexture);
	MenuSprite.setTextureRect(IntRect(0, 0, 1600, 900));

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(600, 100);
	button->setText(L"Створити граф");
	button->setTextSize(25);
	button->setSize(400, 100);
	button->connect("pressed", [=]() {
		WhatModeNow = 1;
		CreateGraphDialog();
	});
	guiMainMenu.add(button);

	button = theme->load("button");
	button->setPosition(600, 300);
	button->setText(L"Довідник");
	button->setTextSize(25);
	button->setSize(400, 100);
	button->connect("pressed", [=]() {
		WhatModeNow = 2;
	});
	guiMainMenu.add(button);

	button = theme->load("button");
	button->setPosition(600, 500);
	button->setText(L"Про програму");
	button->setTextSize(25);
	button->setSize(400, 100);
	button->connect("pressed", [=]() {
		WhatModeNow = 3;
	});
	guiMainMenu.add(button);

	button = theme->load("button");
	button->setPosition(600, 700);
	button->setText(L"Вихід");
	button->setTextSize(25);
	button->setSize(400, 100);
	button->connect("pressed", [=]() {});
	guiMainMenu.add(button);
}
//

//************************************************//
void LoadGraphConstructor()
{
	font.loadFromFile("res/arial.ttf");
	edgeFont.loadFromFile("res/arial.ttf");
	//window.setFramerateLimit(120);
	//window.setVerticalSyncEnabled(true);
	simpleVertexTexture.loadFromFile("res/simple_vertex.png");
	selectedVertexTexture.loadFromFile("res/selected_vertex.png");
	edgeTexture.loadFromFile("res/edge.png");
	edgeLoopTexture.loadFromFile("res/edge_loop.png");
	arrowTexture.loadFromFile("res/arrow2.png");

	selectedVertexSprite.setTexture(selectedVertexTexture);
	selectedVertexSprite.setTextureRect(IntRect(0, 0, 32, 32));//-----------------------------------------------------------------------------переробити!!!!-------------------------------------------------------------------------//

	edgeSprite.setTexture(edgeTexture);
	edgeSprite.setTextureRect(IntRect(0, 0, 24, 24));

	vertexSprite.setTexture(simpleVertexTexture);
	vertexSprite.setTextureRect(IntRect(0, 0, 32, 32));

	edgeLoopSprite.setTexture(edgeLoopTexture);
	edgeLoopSprite.setTextureRect(IntRect(0, 0, 32, 32));

	arrowSprite.setTexture(arrowTexture);
	arrowSprite.setTextureRect(IntRect(0, 0, 10, 10));

	EdgeWeight.setFont(font);
	EdgeWeight.setCharacterSize(16);
	EdgeWeight.setFillColor(Color::White);

	VertexDegree.setFont(font);
	VertexDegree.setCharacterSize(16);
	VertexDegree.setFillColor(Color::Yellow);

	CentOrPer.setFont(font);
	CentOrPer.setCharacterSize(16);
	CentOrPer.setFillColor(Color::Yellow);

	tabs->setTabHeight(50);
	tabs->setTextSize(14);
	tabs->setPosition(20, 20);
	tabs->add(L"Додати вершину");
	tabs->add(L"Перемістити вершину");
	tabs->add(L"З'єднати вершини");
	tabs->add(L"Видалити вершину");
	tabs->add(L"Видалити ребро");
	tabs->deselect();
	gui.add(tabs);

	tabs->connect("TabSelected", onTabSelected, std::ref(gui));
	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(800, 20);
	button->setText(L"Матриця суміжності");
	button->setTextSize(14);
	button->setSize(170, 50);
	button->connect("pressed", ShowAdjacencyMatrix);
	gui.add(button);

	button = theme->load("button");
	button->setPosition(990, 20);
	button->setText(L"Матрця інцидентності");
	button->setTextSize(14);
	button->setSize(170, 50);
	button->connect("pressed", ShowIncidenceMatrix);
	gui.add(button);

	button = theme->load("button");
	button->setPosition(1400, 20);
	button->setText(L"Інші дії");
	button->setTextSize(14);
	button->setSize(180, 50);
	//button->connect("pressed", Dijkstra_Algorithm);
	gui.add(button);

	tgui::Label::Ptr label = theme->load("label");
	label->setText(L"Оберіть алгоритм:");
	label->setPosition(1180, 20);
	label->setTextSize(14);
	gui.add(label);

	tgui::ComboBox::Ptr comboBox = theme->load("combobox");
	comboBox->setTextSize(14);
	comboBox->setSize(200, 25);
	comboBox->setPosition(1180, 45);
	comboBox->addItem(L"Дейкстри(найкоротший шлях)");
	comboBox->addItem(L"Степінь вершин графа");
	comboBox->addItem(L"Радіус та діаметр");
	comboBox->connect("ItemSelected", [=]() 
	{ 
		switch (comboBox->getSelectedItemIndex())
		{
		case 0:
			Dijkstra_Algorithm();
			break;
		case 1:
			VertexsDegree();
			break;
		case 2:
			RadiusAndDiameter();
			break;
		}
	});
	gui.add(comboBox);
}
//

Texture infoTexture;
Sprite infoSprite;
tgui::TextBox::Ptr infoTextBox = theme->load("TextBox");
int pageNumber = 1;
int pagePosition = 0;

void SetPageNumber(Event event)
{
	pageNumber -= event.mouseWheel.delta;
	if (pageNumber > 0 && pageNumber < 10)
	{
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-00" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber > 9 && pageNumber < 100)
	{
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-0" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber > 99 && pageNumber < 133)
	{
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber <= 0)
	{
		pageNumber = 1;
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-00" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber >= 133)
	{
		pageNumber = 132;
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-" + std::to_string(pageNumber) + ".jpg");
	}
	infoTextBox->setText(std::to_string(pageNumber - 1));
}

void SetPagePosition(Event event)
{
	pagePosition -= event.mouseWheel.delta * 100;
	if (pagePosition >= 0 && pagePosition <= 900)
	{
		infoSprite.setTextureRect(IntRect(0, pagePosition, 1250, 750));
	}
	else if(pagePosition < 0)
	{
		pagePosition = 900;
		infoSprite.setTextureRect(IntRect(0, pagePosition, 1250, 750));
		SetPageNumber(event);
	}
	else
	{
		pagePosition = 0;
		infoSprite.setTextureRect(IntRect(0, pagePosition, 1250, 750));
		SetPageNumber(event);
	}
}

void InfoTabSelected(tgui::Gui& guiInformer, String selectedTab)
{
	if (selectedTab == L"     Зміст    ")
	{
		pageNumber = 2;
	}
	else if (selectedTab == L"   Введення в \n теорію графів")
	{
		pageNumber = 5;
	}
	else if (selectedTab == L"   Ізоморфізм \n     графів    ")
	{
		pageNumber = 17;
	}
	else if (selectedTab == L"   Пакування, \n   покриття... ")
	{
		pageNumber = 24;
	}
	else if (selectedTab == L"   Правильна  \n  розфарбовка \n     графів    ")
	{
		pageNumber = 48;
	}
	else if (selectedTab == L"  Мінімальні  \n   островні   \n    дерева     ")
	{
		pageNumber = 55;
	}
	else if (selectedTab == L"Цикли та коцикли")
	{
		pageNumber = 67;
	}
	else if (selectedTab == L"  Сильно зв'язні \nкомпоненти орграфа")
	{
		pageNumber = 84;
	}
	else if (selectedTab == L"Найкоротший шлях")
	{
		pageNumber = 98;
	}
	else if (selectedTab == L"Мережеві задачі")
	{
		pageNumber = 107;
	}
	else if (selectedTab == L" Індивідуальні\nдомашні завдання")
	{
		pageNumber = 115;
	}
	
	if (pageNumber > 0 && pageNumber < 10)
	{
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-00" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber > 9 && pageNumber < 100)
	{
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-0" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber > 99 && pageNumber < 133)
	{
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber <= 0)
	{
		pageNumber = 1;
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-00" + std::to_string(pageNumber) + ".jpg");
	}
	else if (pageNumber >= 133)
	{
		pageNumber = 132;
		infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-" + std::to_string(pageNumber) + ".jpg");
	}
	infoTextBox->setText(std::to_string(pageNumber - 1));
}


//************************************************//
void LoadInformer()
{
	infoTextBox->setSize(60, 20);
	infoTextBox->setTextSize(14);
	infoTextBox->setText(std::to_string(pageNumber - 1));
	infoTextBox->setPosition(770, 860);
	guiInformer.add(infoTextBox);

	tgui::Tab::Ptr infoTabs = tgui::Tab::create();
	infoTabs->setTabHeight(50);
	infoTabs->setTextSize(14);
	infoTabs->setPosition(75, 20);
	infoTabs->add(L"     Зміст    ");
	infoTabs->add(L"   Введення в \n теорію графів");
	infoTabs->add(L"   Ізоморфізм \n     графів    ");
	infoTabs->add(L"   Пакування, \n   покриття... ");
	infoTabs->add(L"   Правильна  \n  розфарбовка \n     графів    ");
	infoTabs->add(L"  Мінімальні  \n   островні   \n    дерева     ");
	infoTabs->add(L"Цикли та коцикли");
	infoTabs->add(L"  Сильно зв'язні \nкомпоненти орграфа");
	infoTabs->add(L"Найкоротший шлях");
	infoTabs->add(L"Мережеві задачі");
	infoTabs->add(L" Індивідуальні\nдомашні завдання");
	infoTabs->deselect();
	infoTabs->connect("TabSelected", InfoTabSelected, std::ref(guiInformer));
	guiInformer.add(infoTabs);
	infoTexture.loadFromFile("res/GraphTheoryLectures/GraphTheoryLectures-page-00" + std::to_string(pageNumber) + ".jpg");
	infoSprite.setTexture(infoTexture);
	infoSprite.setTextureRect(IntRect(0, pagePosition, 1250, 750));
	infoSprite.setPosition(160, 100);
}
//



//************************************************//
void LoadAboutProgram()
{
	tgui::TextBox::Ptr textBox = theme->load("TextBox");
	textBox->setSize(1500, 850);
	textBox->setScrollbar(theme->load("scrollbar"));
	textBox->setTextSize(18);
	textBox->setPosition(50, 50);
	textBox->setReadOnly();
	textBox->setText(L"Інформація про програму...");
	guiAboutProgram.add(textBox);
}
//

void EventNow(Event event)
{
	switch (WhatModeNow)
	{
	case 0:
		guiMainMenu.handleEvent(event);
		break;
	case 1:
		gui.handleEvent(event);
		break;
	case 2:
		guiInformer.handleEvent(event);
		if (event.type == sf::Event::MouseWheelMoved)
		{
			SetPagePosition(event);
		}
		break;
	case 3:
		guiAboutProgram.handleEvent(event);
		break;
	}
}

void DrawModeNow(Event event)
{
	switch (WhatModeNow)
	{
	case 0:
		window.draw(MenuSprite);
		guiMainMenu.draw();
		break;
	case 1:
		update(event);
		Drawing();
		gui.draw();
		break;
	case 2:
		guiInformer.draw();
		window.draw(infoSprite);
		break;
	case 3:
		guiAboutProgram.draw();
		break;
	}
}


int main()
{
	/*
	Clock timer;
	float time;

	time = timer.getElapsedTime().asMicroseconds();
	timer.restart();
	time /= 5000;
	*/
	LoadMainMenu();
	LoadInformer();
	LoadAboutProgram();
	LoadGraphConstructor();

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			EventNow(event);
		}

		if (event.type == Event::KeyReleased)
		{
			if (event.key.code == Keyboard::Escape)
			{
				WhatModeNow = 0;
			}
		}

		window.clear(Color(120, 120, 120));
		
		DrawModeNow(event);

		window.display();
	}
	return 0;
}

