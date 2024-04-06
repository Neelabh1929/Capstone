const wrapper = document.querySelector('.wrapper');
const backBtn = document.querySelector('.back-btn');
const menuBtn = document.querySelector('.menu-btn');
const defaultCategoryImage = "design1.jpg"; 
// localStorage.clear();

const toggleScreen = () => {
    wrapper.classList.toggle('show-category');
};

menuBtn.addEventListener('click', toggleScreen);
backBtn.addEventListener('click', toggleScreen);

const addTaskBtn = document.querySelector('.add-task-btn');
const addTaskForm = document.querySelector('.add-task');
const blackBackdrop = document.querySelector('.black-backdrop');

const toggleAddTaskForm = () => {
    addTaskForm.classList.toggle('active');
    blackBackdrop.classList.toggle('active');
    addTaskBtn.classList.toggle('active');
};

addTaskBtn.addEventListener('click', toggleAddTaskForm);
blackBackdrop.addEventListener('click', toggleAddTaskForm);

let categories = JSON.parse(localStorage.getItem('categories')) ||  [
    {id: 1, 
        title: 'Personal',
        img: 'boss.png',
        deletable: false 
    },
    {id: 2, 
        title: 'Work',
        img: 'suitcase.png',
        deletable: false 
    },
    {id: 3, 
        title: 'Shopping',
        img: 'shopping-cart.png',
        deletable: false 
    },
    {id: 4, 
        title: 'Coding',
        img: 'programming.png',
        deletable: false 
    },
    {id: 5, 
        title: 'Health',
        img: 'better-health.png',
        deletable: false 
    },
    {id: 6, 
        title: 'Fitness',
        img: 'weightlifting.png',
        deletable: false 
    },
    {id: 7, 
        title: 'Education',
        img: 'reading.png',
        deletable: false 
    },
    {id: 8, 
        title: 'Finance',
        img: 'salary.png',
        deletable: false 
    },
];
let nextCategoryId = categories.length ? categories[categories.length - 1].id + 1 : 1;
categories.forEach(category => delete category.deletable);

let tasks = [];

let selectedCategory = categories[0];

const categoriesContainer = document.querySelector('.categories');
const categoryTitle = document.querySelector('.category-title');
const totalCategoryTasks = document.querySelector('.category-tasks');
const categoryImg = document.querySelector('#category-img');
const totaltasks = document.querySelector('.totalTasks');

const calculateTotal = () => {
    const normalizedSelectedCategory = selectedCategory.title.toLowerCase();
    const categoryTasksCount = tasks.filter((task) => task.category.toLowerCase() === normalizedSelectedCategory).length;
    totalCategoryTasks.innerHTML = `${categoryTasksCount} Tasks`;
    totaltasks.innerHTML = `${tasks.length}`;
};

const renderCategories = () => {
    categoriesContainer.innerHTML = '';
    categories.forEach((category) => {
        const categoryTaskCount = tasks.filter((task) => task.category.toLowerCase() === category.title.toLowerCase()).length;
        const div = document.createElement('div');
        div.className = 'category';
        div.innerHTML = `
            <div class="left">
                <img src="img/${category.img}" alt="${category.title}">
                <div class="content">
                    <h1>${category.title}</h1>
                    <p>${categoryTaskCount} Tasks</p>
                    <button class="deleteBtn" data-id="${category.id}">🗑️</button>            
                </div>
            </div>
            
        `;

        div.addEventListener('click', () => selectCategory(category));

        categoriesContainer.appendChild(div);
        
    });
    
    document.querySelectorAll('.deleteBtn').forEach(button => {
        button.addEventListener('click', function(event) {
            event.stopPropagation(); 
            deleteCategory(this.getAttribute('data-id'));
        });
    });
};

const selectCategory = (category) => {
    wrapper.classList.add('show-category');
    selectedCategory = category;
    categoryTitle.innerHTML = category.title;
    categoryImg.src = `img/${category.img}`;
    calculateTotal();
    renderTasks();
};

const tasksContainer = document.querySelector('.tasks');
const checkReminders = (tasks) => {
    const currentTime = new Date();
    tasks.forEach(task => {
        const dueTime = new Date(task.date);
        const timeDifference = dueTime.getTime() - currentTime.getTime();

        if (timeDifference > 0 
            ) {
            setTimeout(() => {
                alert(`Reminder: Task "${task.task}" is due soon!`);
            }, timeDifference);
        }
    });
};

const renderTasks = () => {
    const normalizedSelectedCategory = selectedCategory.title.toLowerCase();
    const categoryTasks = tasks.filter((task) => task.category.toLowerCase() === normalizedSelectedCategory);
    categoryTasks.sort((a, b) => new Date(a.date) - new Date(b.date));
    tasksContainer.innerHTML = categoryTasks.length === 0 ? `<p class="no-task">No tasks for this category</p>` : categoryTasks.map((task) => createTaskHtml(task)).join('');

    addTaskEventListeners();

    renderCategories();
    calculateTotal();

    checkReminders(categoryTasks);
};
const createTaskHtml = (task) => {
    return `
    <div class="task-wrapper" style="display: flex; justify-content: space-between; align-items: center;">
        <div style="flex-grow: 1;">
            <div style="display: flex; justify-content: start; align-items: baseline;">
                <div class="task-date-time" style="margin-right: 20px;">${task.date}      ${task.time}</div>
                </div>
            
            <div class="task-details" style="display: flex; justify-content: start; align-items: baseline;">
                <label class="task" for="${task.id}" style="white-space: nowrap; margin-right: auto;">
                    <input type="checkbox" id="${task.id}" ${task.completed ? 'checked' : ''}>
                    <span class="checkmark" style="margin-right: 5px;"><i class="bx bx-check"></i></span>
                    <p style="margin: 0;">${task.task}</p>
                </label>
            </div>
        </div>
        <div class="delete" style="flex-shrink: 0;"><i class="bx bxs-trash" style="cursor: pointer;"></i></div>
    </div>
    `;
};


// const createTaskHtml = (task) => {
//     return `
//     <div class="task-wrapper">
//         <div class="task-date">${task.date}</div>
//         <div class="task-date">${task.time}</div>

//         <label class="task" for="${task.id}">
//             <input type="checkbox" id="${task.id}" ${task.completed ? 'checked' : ''}>
//             <span class="checkmark"><i class="bx bx-check"></i></span>
//             <p>${task.task}</p>
//         </label>
//         <div class="delete"><i class="bx bxs-trash"></i></div>
//     </div>
//     `;
// };

const addTaskEventListeners = () => {
    document.querySelectorAll('.task-wrapper').forEach((taskWrapper, index) => {
        const checkbox = taskWrapper.querySelector('input[type="checkbox"]');
        checkbox.addEventListener('change', () => toggleTaskCompleted(index));

        const deleteBtn = taskWrapper.querySelector('.delete');
        deleteBtn.addEventListener('click', () => deleteTask(index, taskWrapper));
    });
};

const toggleTaskCompleted = (index) => {
    tasks[index].completed = !tasks[index].completed;
    saveLocal();
};

const deleteTask = (index, taskWrapper) => {
    tasks.splice(index, 1);
    taskWrapper.remove();
    saveLocal();
    calculateTotal(); 
    renderTasks();
};

// saveget from local storage
const saveLocal = () => {
    localStorage.setItem('tasks', JSON.stringify(tasks));
};

const getLocal = () => {
    const localTasks = JSON.parse(localStorage.getItem('tasks'));

    if (localTasks) {
        tasks = localTasks;
    }
};


// category part start
const categorySelect = document.querySelector('#category-select');
const cancelBtn = document.querySelector('.cancel-btn');
const addBtn = document.querySelector('.add-btn');

const taskInput = document.querySelector('#task-input'); 

cancelBtn.addEventListener('click', toggleAddTaskForm);

addBtn.addEventListener('click', () => {
    const task = taskInput.value.trim();
    const category = categorySelect.value;
    const date = document.getElementById('task-date').value;
    const time = document.getElementById('task-time').value; 

    if (!task || !date) {
        alert('Please enter all the fields.');
        return;
    }

    const newTaskId = Date.now();

    const newTask = {
        id: newTaskId,
        task,
        category,
        completed: false,
        date,
        time
    };

    tasks.push(newTask);
    taskInput.value = '';
    document.getElementById('task-time').value = '';
    saveLocal();
    toggleAddTaskForm();
    renderTasks();
});

categories.forEach((category) => {
    const option = document.createElement('option');
    option.value = category.title.toLowerCase();
    option.textContent = category.title;
    categorySelect.appendChild(option);
});
const addNewCategory = () => {
     const newCategoryName = prompt('Enter the name of the new category:');
    
    if (newCategoryName) {
         const newCategory = {
            id: nextCategoryId++,
            title: newCategoryName,
            img: defaultCategoryImage,
            deletable: true
         };
        
         categories.push(newCategory);
        localStorage.setItem('categories', JSON.stringify(categories));
        const option = document.createElement('option');
        option.value = newCategory.title.toLowerCase();
        option.textContent = newCategory.title;
        categorySelect.appendChild(option);
         renderCategories();
    }
};
menuBtn.addEventListener('click', addNewCategory);


const deleteCategory = (id) => {
    const categoryId = Number(id);

    const deletedCategoryIndex = categories.findIndex(category => category.id === categoryId);
    if (deletedCategoryIndex === -1) {
        console.log("Category not found.");
        return;
    }

    const deletedCategory = categories.splice(deletedCategoryIndex, 1)[0];
    localStorage.setItem('categories', JSON.stringify(categories));

    tasks = tasks.filter(task => task.category.toLowerCase() !== deletedCategory.title.toLowerCase());

    localStorage.setItem('categories', JSON.stringify(categories));
    localStorage.setItem('tasks', JSON.stringify(tasks));

    renderCategories();
    renderTasks();
};
// const deleteCategory = (id) => {
//     // Convert id to number as data-id attribute returns a string
//     const categoryId = Number(id);
//     // Filter out the category to be deleted
//     categories = categories.filter(category => category.id !== categoryId);
//     // Update the local storage
//     localStorage.setItem('categories', JSON.stringify(categories));
//     // Re-render the categories
//     renderCategories();
// };
// function toggleDarkMode() {
//     var body = document.getElementById('body');
//     var darkModeIcon = document.getElementById('dark-mode-icon');

//     body.classList.toggle('dark-mode');

//     if (body.classList.contains('dark-mode')) {
//         darkModeIcon.classList.remove('bx-sun');
//         darkModeIcon.classList.add('bx-moon');
//     } else {
//         darkModeIcon.classList.remove('bx-moon');
//         darkModeIcon.classList.add('bx-sun');
//     }
// }
function toggleDarkMode() {
    var body = document.body;
    var darkModeIcon = document.getElementById('dark-mode-icon');

    body.classList.toggle('dark-mode');

    if (body.classList.contains('dark-mode')) {
        darkModeIcon.classList.remove('bx-sun');
        darkModeIcon.classList.add('bx-moon');
    } else {
        darkModeIcon.classList.remove('bx-moon');
        darkModeIcon.classList.add('bx-sun');
    }
}



getLocal();
calculateTotal();
renderCategories();
renderTasks();
