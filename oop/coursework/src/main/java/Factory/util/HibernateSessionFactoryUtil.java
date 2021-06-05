package Factory.util;

import Factory.model.*;
import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;

public class HibernateSessionFactoryUtil
{
    private static SessionFactory sessionFactory;

    private HibernateSessionFactoryUtil() {}

    public static SessionFactory getSessionFactory()
    {
        if (sessionFactory == null)
        {
            Configuration configuration = new Configuration()
                    .configure("hibernate.cfg.xml")
                    .addAnnotatedClass(Person.class)
                    .addAnnotatedClass(Employee.class)
                    .addAnnotatedClass(Specialisation.class)
                    .addAnnotatedClass(Manager.class)
                    .addAnnotatedClass(Client.class)
                    .addAnnotatedClass(Contract.class);
            StandardServiceRegistryBuilder builder = new StandardServiceRegistryBuilder().applySettings(configuration.getProperties());
            sessionFactory = configuration.buildSessionFactory(builder.build());
        }

       return sessionFactory;
    }
}
