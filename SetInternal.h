/**
 * File: SetInternal.h
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * The internal representation of the Object type, along with auxiliary helper types.
 *
 * You should not need to use or modify the contents of this file, though you're welcome
 * to look around a bit if you'd like!
 *
 * Internally, Objects are wrappers around a class hierarchy consisting of a base type
 * SetObject and derived types representing sets and non-sets. This representation
 * essentially models sets as trees: each set is a node, and each of its elements is
 * a child.
 *
 * Fun fact: if you study pure set theory, you can actually get a lot of mileage by
 * modeling sets as trees. Look up "complement of the axiom of infinity" for a fun
 * tour of what happens if you do this, or take Math 161!
 */
#pragma once
#include <string>
#include <memory>
#include <ostream>
#include <set>

namespace SetTheory {
    struct SetObject;
    /* Type: Object
     *
     * An opaque type representing an object, which can either be a set or
     * some other object.
     *
     * Please do not access the fields of this struct. Use our provided helper functions
     * to operate on Objects.
     */
    struct Object {
        std::shared_ptr<const SetTheory::SetObject> impl;
        bool operator< (const Object& rhs) const;    // Needed for std::set

        /* You are not allowed to directly compare Objects against one another for equality.
         * If you get an error on this line, it's probably because you tried writing something
         * like
         *
         *     if (obj1 == obj2) { ... }
         *
         * If that's the case, see if you can find another way to express the idea you're working
         * with.
         */
        bool operator==(const Object& rhs) = delete;
    };

    /* Type: SetObject
     *
     * Polymorphic base type representing a set or an object.
     */
    struct SetObject {
        /* Polymorphic classes need virtual destructors. */
        virtual ~SetObject() = default;

        virtual bool isSet() const = 0;
        virtual std::set<Object> asSet() const = 0;
        virtual std::string toString() const = 0;
    };

    /* Type: ActualSet
     *
     * A type representing an actual set of objects.
     */
    struct ActualSet: public SetObject {
        std::set<Object> theSet;

        explicit ActualSet(const std::set<Object>& s = {}) : theSet(s) {}

        virtual bool isSet() const override;
        virtual std::set<Object> asSet() const override;
        virtual std::string toString() const override;
    };

    /* Type: ActualObject
     *
     * A type representing an honest-to-goodness concrete non-set object.
     */
    struct ActualObject: public SetObject {
        std::string name;

        explicit ActualObject(const std::string& n) : name(n) {}

        virtual bool isSet() const override;
        virtual std::set<Object> asSet() const override;
        virtual std::string toString() const override;
    };

    /* Operator <<
     *
     * Allows you to print out an object to the console for debugging purposes. You can use it like
     * this:
     *
     *    Object obj = // ... //
     *    std::cout << "Object: " << obj << std::endl;
     */
    std::ostream& operator<< (std::ostream& out, const Object& obj);

    /* Given an object, returns whether that object is a set.
     *
     * For example, given an object representing the number 1,
     * this would return false. Given an object representing
     * { 1, 2, 3 }, this function would return true.
     */
    bool isSet(Object o);

    /* Given an object that represents a set, returns a view of
     * that object as a set.
     *
     * For example, suppose you have something like this:
     *
     *    Object o = // ... something you know is a set ... //
     *
     * You could then actually see the contents of that set
     * by writing something like
     *
     *    std::set<Object> S = asSet(o);
     *
     * If you try to convert an object to a set and that object
     * isn't actually a set, this function will trigger an error.
     *
     * Here's a nice way to iterate over all the contents of an
     * Object that you know is a set:
     *
     *    Object o = // ... something you know is a set .. //
     *    for (Object x: asSet(o)) {
     *       // ... do something to x ... //
     *    }
     */
    std::set<Object> asSet(Object o);
}
