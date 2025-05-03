# inspect v1.0.0
Ever wanted to look at a file quickly, possibly before deleting it?

There are a few options for this:

- An editor (such as Vim)
- A viewer (such as less or cat)
- A file browser (such as Commander One)

Now, what if you wanted to just *decide* what to do while looking at it, without using something bloaty like an editor, or having to context-switch with something like a viewer, or switching up your whole workflow to use one more big program to manage your files?

`inspect`, in my humble opinion, fits that category. It's lightweight enough (the compiled binary is only 40kB) and allows you to perform several (extensible) actions while offering a Vim-like view.

"But doesn't Vim offer a read-only view?"

Yes, but I designed this tool because A) I didn't want to be limited in the scope of a Vim extension or alias in the future, and B) programming is fun. With that in mind, it was designed to run fast, so invoking it is also fast, so you don't spend unnecessary time typing:
```sh
i README.md
```

With that aside...

## Installation
Windows support is not planned for the future by me (@fuguenot), but any PRs are welcome.

Clone the repository, then run `./install.sh`:
```sh
git clone https://github.com/fuguenot/inspect.git
./install.sh
```
Once you reload your shell, `i` should be available as a command.

Happy terminating!
