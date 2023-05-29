# StatisticTree
### Задание
Имеется дерево каталогов в файловой системе. При выборе папки показывается статистика по ней:</br> 
 • количество файлов, общий размер и средний размер для каждой группы файлов (группировать по расширению и отдельной строкой - для всех) - рекурсивно по всей папке. </br> 
 • количество подкаталогов - только для данной папки.</br> 
 
Интерфейс пользователя должен отображать статус всех асинхронных операций -
построение дерева каталогов, подсчет статистки по каталогу.
Подсчёт статистики должен производиться в фоновом потоке, интерфейс
пользователя не должен замирать или блокироваться.
 
### Описание программы: </br>
Класс ExtensionNode рассчитывает статистику для каждого расширения файлов</br>
Класс StatisticDir рассчитывает количество подпапок и управляет расчётом всех ExtensionNode</br>
Класс StatisticModel и FilesystemModel отображают табличную часть статистики и древовидную часть иерархической модели файловой системы.</br>
Кдасс формы WidgetTree - набор виджетов для визуального взаимодействия с пользователем.