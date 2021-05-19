package Factory.service;

import Factory.dao.SpecialisationDAO;
import Factory.model.*;
import java.util.List;

public class SpecialisationService
{
    private static SpecialisationDAO specialisationDao;

    public SpecialisationService()
    {
        specialisationDao = new SpecialisationDAO();
    }

    public void persist(Specialisation entity) {
        specialisationDao.openCurrentSessionwithTransaction();
        specialisationDao.persist(entity);
        specialisationDao.closeCurrentSessionwithTransaction();
    }

    public void update(Specialisation entity) {
        specialisationDao.openCurrentSessionwithTransaction();
        specialisationDao.update(entity);
        specialisationDao.closeCurrentSessionwithTransaction();
    }

    public Specialisation findById(int id)
    {
        specialisationDao.openCurrentSession();
        Specialisation manager = specialisationDao.findById(id);
        specialisationDao.closeCurrentSession();
        return manager;
    }

    public Specialisation findByName(String name)
    {
        specialisationDao.openCurrentSession();
        Specialisation manager = specialisationDao.findByName(name);
        specialisationDao.closeCurrentSession();
        return manager;
    }

    public void delete(int id) {
        specialisationDao.openCurrentSessionwithTransaction();
        Specialisation manager = specialisationDao.findById(id);
        specialisationDao.delete(manager);
        specialisationDao.closeCurrentSessionwithTransaction();
    }

    public List<Specialisation> findAll() {
        specialisationDao.openCurrentSession();
        List<Specialisation> managers = specialisationDao.findAll();
        specialisationDao.closeCurrentSession();
        return managers;
    }

    public void deleteAll() {
        specialisationDao.openCurrentSessionwithTransaction();
        specialisationDao.deleteAll();
        specialisationDao.closeCurrentSessionwithTransaction();
    }

    public SpecialisationDAO specialisationDao()
    {
        return specialisationDao;
    }
}