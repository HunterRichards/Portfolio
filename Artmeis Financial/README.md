# CS-305 Artemis Financial
<br><b>Briefly summarize your client, Artemis Financial, and their software requirements. Who was the client? What issue did they want you to address?</b></br>

Artemis Financial was looking for a secure way to validate files using a secure hashing algorithm. It was my job to identify the correct algorithm for their use case, implement the algorithm, and showcase its functioning state. Artemis also requested that the communication between client and server utilize strong security policies. The process of securing the site involved creating a HTTPS certificate (for secure handshakes), dependency verification through static testing, and secondary testing which implemented secure coding practices.

<br><b>What did you do very well when you found your client’s software security vulnerabilities? Why is it important to code securely? What value does software security add to a company’s overall wellbeing?</b></br>

I believe I implemented the algorithm cipher very well. This involved choosing a hashing algorithm, SHA-256 in this instance, and then implementing the hashing algorithm on a string input. The data was then converted from a hash value to a hexadecimal one for the valid checksum.
Coding securely is essential for data integrity, company integrity, and a better/safer consumer/business experience. This, in essence, protects the company’s well-being by mitigating data-breaches and malicious actors, which could cause litigious action against the company (not including the irreparable harm to the company’s identity/brand).

<br><b>What part of the vulnerability assessment was challenging or helpful to you?</b></br>

The algorithm choice and implementation (both for Project 1 where AES-256 was chosen & Project 2 where SHA-256 was chosen) were what I found most useful. To see it work in practice, versus theory, helped me obtain a better grasp of the concepts in this course.

<br><b>How did you increase layers of security? In the future, what would you use to assess vulnerabilities and decide which mitigation techniques to use?</b></br>

The approach used for Artemis financial included a layered approach involving algorithm ciphers, certificate generation for secure handshakes, secure communication between client and server, primary testing involving a dependency check (static test), secondary testing to identify vulnerabilities found within the code through manual review. All these methods highlight the importance of a layered continuous approach for secure programming. The techniques learned in this course are definitely going to be an important consideration for future programming endeavors.

<br><b>How did you make certain the code and software application were functional and secure? After refactoring the code, how did you check to see whether you introduced new vulnerabilities?</b></br>

The program was made to be functional and secure through a manual review and algorithm ciphers. Likewise, a certificate was generated to allow for a secure web connection between the client and server. The program was then run and subsequently showcased to provide evidence of its secure elements.
New vulnerabilities were detected in a two-step process. First the refactored code underwent static testing to identify vulnerable libraries. The libraries were manually check/reviewed for potential defects against the NIST software vulnerabilities database. Then the code was manually checked for potential vulnerabilities; and then subsequently altered to prevent as such.

<br><b>What resources, tools, or coding practices did you use that might be helpful in future assignments or tasks?</b></br>

This course, in general, provided great insight into the practices used for secure coding. My perception of security has changed from being a standard afterthought, to now being an integrative/procedural process. Security should be an ongoing evolution, not only for the IT team, but for the entire company. Careful planning is essential to provide secure solutions where they are needed most.

<br><b>Employers sometimes ask for examples of work that you have successfully completed to show your skills, knowledge, and experience. What might you show future employers from this assignment?</b></br>

I believe the development and skills obtained from this course are invaluable for my professional journey in software engineering. This class has created the groundwork on identifying key security concepts and practices. Concepts including static reporting, understanding secure programming procedures and the software security lifecycle (security work in tandem with development versus an afterthought), using best industry practices for software security, identifying different types of algorithm ciphers and their best use cases, implementing algorithm ciphers and showcasing their practical use-cases, understanding the architectural review process (Input Validation, APIs, Cryptography, Client/Server relationships, Code Error handling, Code Quality, and Encapsulation) to identify potential security weak points in a software’s design, and much more.
Many industries fail to meet these minimum-security requirements; especially when sensitive personal information is involved. Thus, from the concepts learned in this course, I hope to be part of the industry push towards secure programming and software security in general.