package test;

import ContactService.Contact;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public class ContactTest {
    private Contact contact;

    @BeforeEach
    public void setUp() {
        contact = new Contact("1234567890", "John", "Doe", "1234567890", "Sample Address");
    }

    @Test
    public void testValidContactCreation() {
        Assertions.assertEquals("1234567890", contact.getContactId());
        Assertions.assertEquals("John", contact.getFirstName());
        Assertions.assertEquals("Doe", contact.getLastName());
        Assertions.assertEquals("1234567890", contact.getPhone());
        Assertions.assertEquals("Sample Address", contact.getAddress());
    }

    @Test
    public void testInvalidContactCreation() {
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact(null, "John", "Doe", "1234567890", "Sample Address"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("12345678901", "John", "Doe", "1234567890", "Sample Address"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("1234567890", null, "Doe", "1234567890", "Sample Address"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("1234567890", "John", null, "1234567890", "Sample Address"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("1234567890", "John", "Doe", "1234", "Sample Address"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("1234567890", "John", "Doe", "1234567890", null));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("1234567890", "John", "Doe", "abc123def4", "Sample Address"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> new Contact("1234567890", "John", "Doe", "1234567890", "Sample Address Sample Address Sample Address"));
    }

    @Test
    public void testValidContactFieldUpdates() {
        contact.setFirstName("Jane");
        Assertions.assertEquals("Jane", contact.getFirstName());

        contact.setLastName("Smith");
        Assertions.assertEquals("Smith", contact.getLastName());

        contact.setPhone("9876543210");
        Assertions.assertEquals("9876543210", contact.getPhone());

        contact.setAddress("New Address");
        Assertions.assertEquals("New Address", contact.getAddress());
    }

    @Test
    public void testInvalidContactFieldUpdates() {
        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setFirstName(null));
        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setFirstName("John123"));

        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setLastName(null));
        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setLastName("Doe123"));

        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setPhone(null));
        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setPhone("1234"));
        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setPhone("abc123def4"));

        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setAddress(null));
        Assertions.assertThrows(IllegalArgumentException.class, () -> contact.setAddress("Sample Address Sample Address Sample Address"));
    }
}