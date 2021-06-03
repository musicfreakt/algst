import Factory.gui.ContractWindow;
import Factory.gui.Menu;

import java.util.logging.Logger;

/** Главный класс приложения */
public class App
{
    /** Логгер класса */
    private static final Logger log = Logger.getLogger(ContractWindow.class.getName());

    /** Главная функция */
    public static void main(String[] args)
    {
        log.info("Запуск программы");
        Menu m = new Menu();
        m.show();
    }
}


