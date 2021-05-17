package Factory;

import java.util.*;

import org.hibernate.*;
import org.hibernate.cfg.*;

public class FactoryHandler
{
    private List<Client> clients = null;
    private List<Employee> workers = null;
    private List<Manager> managers = null;
    private List<Specialisation> specialisations= null;
    private List<Contract> contracts = null;

    private SessionFactory factory;
    private Session session;

    public FactoryHandler()
    {
        System.out.println("Time to check save data!");
        System.out.println("Loading...");
        loadData();
    }

    public void loadData()
    {
        factory = new Configuration()
                .configure("META-INF/hibernate.cfg.xml")
                .addAnnotatedClass(Person.class)
                .addAnnotatedClass(Specialisation.class)
                .addAnnotatedClass(Employee.class)
                .addAnnotatedClass(Manager.class)
                .addAnnotatedClass(Client.class)
                .addAnnotatedClass(Contract.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        try
        {
            specialisations = loadSpecialisations();
            clients = loadClients();
            workers = loadWorkers();
            managers = loadManagers();
            contracts = loadContracts();
        }
        finally {
            factory.close();
        }
    }

    public List<Specialisation> loadSpecialisations()
    {
        List<Specialisation> loadedSpecialisations = null;
        factory = new Configuration()
                .configure("META-INF/hibernate.cfg.xml")
                .addAnnotatedClass(Specialisation.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        try
        {
            session.beginTransaction();
            loadedSpecialisations = new ArrayList<Specialisation>(session.createCriteria(Specialisation.class).list() );
            session.getTransaction().commit();
        }
        catch(Throwable t)
        {
            System.out.println(t);
        }
        finally
        {
            factory.close();
        }
        return removeCopies(loadedSpecialisations);
    }

    public List<Client> loadClients()
    {
        List<Client> loadedClients = null;
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Client.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        try
        {
            session.beginTransaction();
            loadedClients = new ArrayList<Client>( session.createCriteria(Client.class).list() );
            session.getTransaction().commit();
        }
        catch(Throwable t)
        {
            System.out.println(t);
        }
        finally
        {
            factory.close();
        }
        return removeCopies(loadedClients);
    }

    public List<Employee> loadWorkers()
    {
        List<Employee> loadedWorkers = null;
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Employee.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        try
        {
            session.beginTransaction();
            loadedWorkers = new ArrayList<Employee>( session.createCriteria(Employee.class).list() );
            session.getTransaction().commit();
        }
        catch(Throwable t)
        {
            System.out.println(t);
        }
        finally
        {
            factory.close();
        }
        return removeCopies(loadedWorkers);
    }

    public List<Manager> loadManagers()
    {
        List<Manager> loadedManagers = null;
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Manager.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        try
        {
            session.beginTransaction();
            loadedManagers = new ArrayList<Manager>( session.createCriteria(Manager.class).list() );
            session.getTransaction().commit();
        }
        catch(Throwable t)
        {
            System.out.println(t);
        }
        finally
        {
            factory.close();
        }
        return removeCopies(loadedManagers);
    }

    public List<Contract> loadContracts()
    {
        List<Contract> loadedContracts = null;
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Contract.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        try
        {
            session.beginTransaction();
            loadedContracts = new ArrayList<Contract>( session.createCriteria(Contract.class).list() );
            session.getTransaction().commit();
        }
        catch(Throwable t)
        {
            System.out.println(t);
        }
        finally
        {
            factory.close();
        }
        return removeCopies(loadedContracts);
    }

    public <T> void saveList(List<T> list)
    {
        for(T el : list)
            saveData(el);
    }

    public <T> void saveData(T data)
    {
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Person.class)
                .addAnnotatedClass(Specialisation.class)
                .addAnnotatedClass(Employee.class)
                .addAnnotatedClass(Manager.class)
                .addAnnotatedClass(Client.class)
                .addAnnotatedClass(Contract.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        session.beginTransaction();
        session.saveOrUpdate(data);
        session.getTransaction().commit();
    }

    public <T> void removeList(List<T> list)
    {
        for(T el : list)
            removeData(el);
    }

    public <T> void removeData(T data)
    {
        factory = new Configuration()
                .configure("hibernate.cfg.xml")
                .addAnnotatedClass(Person.class)
                .addAnnotatedClass(Specialisation.class)
                .addAnnotatedClass(Employee.class)
                .addAnnotatedClass(Manager.class)
                .addAnnotatedClass(Client.class)
                .addAnnotatedClass(Contract.class)
                .buildSessionFactory();

        session = factory.getCurrentSession();
        session.beginTransaction();
        session.delete(data);
        session.getTransaction().commit();
    }

    private <T> List<T> removeCopies(List<T> list)
    {
        List<T> resultList = new ArrayList<T>();
        for(T el : list)
        {
            if(!resultList.contains(el))
                resultList.add(el);
        }
        return resultList;
    }
}
