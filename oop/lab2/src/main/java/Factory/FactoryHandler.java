package Factory;

import java.util.List;

import org.hibernate.*;
import org.hibernate.cfg.*;
import javax.persistence.*;

public class FactoryHandler
{
    private List<Client> clients = null;
    private List<Employee> workers = null;
    private List<Specialisation> specialisations= null;
    private List<Contract> contracts = null;

//    private SessionFactory factory;
//    private Session session;

    public FactoryHandler()
    {
        System.out.println("Time to check save data!");
        System.out.println("Loading...");
//        loadData();
    }


//    public void loadData()
//    {
//        factory = new Configuration()
//                .configure("hibernate.cfg.xml")
//                .addAnnotatedClass(Racer.class)
//                .addAnnotatedClass(Team.class)
//                .addAnnotatedClass(Track.class)
//                .addAnnotatedClass(Competition.class)
//                .buildSessionFactory();
//
//        session = factory.getCurrentSession();
//        try
//        {
//            racers = loadRacers();
//            teams = loadTeams();
//            tracks = loadTracks();
//            comps = loadComps();
//        }
//        finally {
//            factory.close();
//        }
//    }

//    public List<Racer> loadRacers()
//    {
//        List<Racer> loadedRacers = null;
//        factory = new Configuration()
//                .configure("hibernate.cfg.xml")
//                .addAnnotatedClass(Racer.class)
//                .buildSessionFactory();
//
//        session = factory.getCurrentSession();
//        try
//        {
//            session.beginTransaction();
//            loadedRacers = new ArrayList<Racer>( session.createCriteria(Racer.class).list() );
//            session.getTransaction().commit();
//        }
//        catch(Throwable t)
//        {
//            System.out.println(t);
//        }
//        finally
//        {
//            factory.close();
//        }
//        return removeCopies(loadedRacers);
//    }
//
//    public List<Team> loadTeams()
//    {
//        List<Team> loadedTeams = null;
//        factory = new Configuration()
//                .configure("hibernate.cfg.xml")
//                .addAnnotatedClass(Team.class)
//                .buildSessionFactory();
//
//        session = factory.getCurrentSession();
//        try
//        {
//            session.beginTransaction();
//            loadedTeams = new ArrayList<Team>( session.createCriteria(Team.class).list() );
//            session.getTransaction().commit();
//        }
//        catch(Throwable t)
//        {
//            System.out.println(t);
//        }
//        finally
//        {
//            factory.close();
//        }
//        return removeCopies(loadedTeams);
//    }
//
//    public List<Track> loadTracks()
//    {
//        List<Track> loadedTracks = null;
//        factory = new Configuration()
//                .configure("hibernate.cfg.xml")
//                .addAnnotatedClass(Track.class)
//                .buildSessionFactory();
//
//        session = factory.getCurrentSession();
//        try
//        {
//            session.beginTransaction();
//            loadedTracks = new ArrayList<Track>( session.createCriteria(Track.class).list() );
//            session.getTransaction().commit();
//        }
//        catch(Throwable t)
//        {
//            System.out.println(t);
//        }
//        finally
//        {
//            factory.close();
//        }
//        return removeCopies(loadedTracks);
//    }
//
//    public List<Competition> loadComps()
//    {
//        List<Competition> loadedComps = null;
//        factory = new Configuration()
//                .configure("hibernate.cfg.xml")
//                .addAnnotatedClass(Competition.class)
//                .buildSessionFactory();
//
//        session = factory.getCurrentSession();
//        try
//        {
//            session.beginTransaction();
//            loadedComps = new ArrayList<Competition>( session.createCriteria(Competition.class).list() );
//            session.getTransaction().commit();
//        }
//        catch(Throwable t)
//        {
//            System.out.println(t);
//        }
//        finally
//        {
//            factory.close();
//        }
//        return removeCopies(loadedComps);
//    }

//    public <T> void saveList(List<T> list)
//    {
//        for(T el : list)
//            saveData(el);
//    }

//    public <T> void saveData(T data)
//    {
//        factory = new Configuration()
//                .configure("hibernate.cfg.xml")
//                .addAnnotatedClass(Racer.class)
//                .addAnnotatedClass(Team.class)
//                .addAnnotatedClass(Track.class)
//                .addAnnotatedClass(Competition.class)
//                .buildSessionFactory();
//
//        session = factory.getCurrentSession();
//        session.beginTransaction();
//        session.saveOrUpdate(data);
//        session.getTransaction().commit();
//    }

    /*public <T> void removeList(List<T> list)
    {
        for(T el : list)
            removeData(el);
    }

    public <T> void removeData(T data)
    {
        factory = new Configuration()
								.configure("hibernate.cfg.xml")
                                .addAnnotatedClass(Racer.class)
                                .addAnnotatedClass(Team.class)
                                .addAnnotatedClass(Track.class)
                                .addAnnotatedClass(Competition.class)
                                .buildSessionFactory();

        session = factory.getCurrentSession();
        session.beginTransaction();
        session.delete(data);
        session.getTransaction().commit();
    }*/

//    private <T> List<T> removeCopies(List<T> list)
//    {
//        List<T> resultList = new ArrayList<T>();
//        for(T el : list)
//        {
//            if(!resultList.contains(el))
//                resultList.add(el);
//        }
//        return resultList;
//    }
//}
}
