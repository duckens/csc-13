Здравствуйте!

Как работает код аллокатора:

	Аллокатор строит в блоке памяти дерево, каждая его вершина начинается с записи mcb (24 байта).
	Программа не рассчитана на некорректный ввод.

Структура дерева:

	Вся память (занятая и свободная) хранится в листьях. Любой лист - это либо занятый блок, либо свободный блок.
	Остальные вершины (не листья) - служебные
	Вершины бывают 3-х типов:
	MCB_TYPE_ALLOCATED и MCB_TYPE_FREE - это листья, соответственно содержашие либо выделенные
	куски памяти, либо свободные
	и MCB_TYPE_COMPOSITE - все остальные (т.е. не листья).


	У каждой вершины либо нет ни одного ребенка, либо есть оба. Есть ситуация в программе когда у
	вершины могут начать совпадать указатели на правого и левого ребенка (у вершины как бы остался
	один ребенок), но в этом же месте вызывается update который такую вершину удалит.

	В начале памяти находится root (корень) - mcb запись, которая создаетс с самого начала и не удаляется никогда.

	Верно следующее: всегда выполнено следующее (в смысле адреса):
	родитель < левый_ребенок < правый_ребенок
	Дерево строится всегда так, что от родителя до левого ребенка расстояние равно 0. Однако это может испортиться
	в одном частном случае функции merge.
	Свободный лист знает свой размер.
	Занятый лист не знает своего размера, он определяет его находя в дереве следующую справа вершину.

Структура mcb:

	Запись mcb содержит следующее:
	struct mcb{
		unsigned char type; // тип вершины (один из трех, описанных выше)
		int freeSpace; 	// количество свободной памяти внутри (эта величина для
						// не листьев всегда равна сумме аналогичных записей детей).
						// У занятого блока равна 0, у свободного количеству памяьи в нем.
		int maxBlock; 	// размер максимального блока внутри. (эта величина для
						// не листьев всегда равна std::max аналогичных записей детей).
						// У занятого блока равна 0, у свободного количеству памяьи в нем.
		mcb* child_l;	// указатель на левого ребенка
		mcb* child_r;	// указатель на правого ребенка
		mcb* parent;	// указатель на родителя
	};

	Размер 24 байта. Думаю, что его можно сократить не храня freeSpace и maxBlock - их можно каждый раз вычислять.

Процесс выделения:

	1) проверка что искомое можно выделить (достаточно посмотреть на корень)
	2) поиск вниз по дереву нужного листа (при этом приоритет отдается ребенку в котором maxBlock меньше)
	3) выделение в листе

	Выделение в листе:
	а) Если в листе есть size + 48 байт (на 2 записи mcb) - то найденный лист перестает быть листом -
	он получает 2 ребенка левый ребенок - лист с занятым местом, правый - лист с оставшимся свободным
	б) Если в листе нет size + 48 байт (т.е. есть сколько-то от size до size + 47), то пользователю
	выделяется все это место, лист становится полностью занятым

Процесс освобождения:

	1) Ищется указанное место вниз от корня (чтобы проверить что такое место и правда указывает на занятый кусок памяти)
	Есть 2 режима: строгий и нестрогий. В строгом - считается что нашли - если и правда указывает
	точно на начало, в нестрогом - если попадает в тело занятого куска.
	2) Если нашли - удаляется (лист становится типа MCB_TYPE_FREE)
	3) Если у этого листа есть ближайший к нему лист, который тоже сбободен - выполняется merge. Я прикладываю картинку
	(нарисованую от руки) поясняющую как работает merge.
	4) Прогонется update вверх по дереву от указанного места.



(я понимаю что неверное не слишком понятно описал работу дерева, скажите пожалуйста что надо описать подробнее)

С уважением,
Андрей Бороздин
