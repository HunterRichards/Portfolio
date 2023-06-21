package test;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import ContactService.Contact;
import ContactService.ContactService;

public class ContactServiceTest {
    private ContactService contactService;

    @BeforeEach
    public void setup() {
        contactService = new ContactService();
    }

    @Test
    public void testAddContact() {
        contactService.addContact("1", "John", "Doe", "1234567890", "Address 1");

        Contact contact = contactService.getContact("1");
        Assertions.assertNotNull(contact);
        Assertions.assertEquals("John", contact.getFirstName());
        Assertions.assertEquals("Doe", contact.getLastName());
        Assertions.assertEquals("1234567890", contact.getPhone());
        Assertions.assertEquals("Address 1", contact.getAddress());
    }

    @Test
    public void testDeleteContact() {
        contactService.addContact("1", "John", "Doe", "1234567890", "Address 1");

        contactService.deleteContact("1");

        Contact contact = contactService.getContact("1");
        Assertions.assertNull(contact);
    }

    @Test
    public void testUpdateContactField() {
        contactService.addContact("1", "John", "Doe", "1234567890", "Address 1");

        contactService.updateContactField("1", "firstName", "Jane");

        Contact contact = contactService.getContact("1");
        Assertions.assertNotNull(contact);
        Assertions.assertEquals("Jane", contact.getFirstName());

        contactService.updateContactField("1", "number", "9876543210");

        contact = contactService.getContact("1");
        Assertions.assertNotNull(contact);
        Assertions.assertEquals("9876543210", contact.getPhone());
    }

    @Test
    public void testUpdateContactField_InvalidField() {
        contactService.addContact("1", "John", "Doe", "1234567890", "Address 1");

        contactService.updateContactField("1", "invalidField", "Value");

        Contact contact = contactService.getContact("1");
        Assertions.assertNotNull(contact);
        Assertions.assertEquals("John", contact.getFirstName());
        Assertions.assertEquals("Doe", contact.getLastName());
        Assertions.assertEquals("1234567890", contact.getPhone());
        Assertions.assertEquals("Address 1", contact.getAddress());
    }
}